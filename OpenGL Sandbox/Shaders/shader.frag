#version 330 core

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;

layout(location = 0) out vec4 colour;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
    vec3 colour;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};

struct PointLight
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float edge;
};

struct OmniShadowMap
{
    samplerCube shadowMap;
    float farPlane;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;
uniform bool useOmniShadow;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture; // GL_TEXTURE0
uniform sampler2D directionalShadowMap;
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform Material material;

uniform vec3 eyePosition;

int sampleRate = 2;

vec3 sampleOffsetDirections[20] = vec3[]
(
    vec3(1, 1, 1),  vec3(1, -1, 1), vec3(-1, -1, 1),    vec3(-1, 1, 1),
    vec3(1, 1, -1), vec3(1, -1, -1),vec3(-1, -1,-1),    vec3(-1, 1, -1),
    vec3(1, 1, 0),  vec3(1, -1, 0), vec3(-1, -1, 0),    vec3(-1, 1, 0),
    vec3(1, 0, 1),  vec3(-1, 0, 1), vec3(1, 0, -1),     vec3(-1, 0, -1),
    vec3(0, 1, 1),  vec3(0, -1, 1), vec3(0, -1, -1),    vec3(0, 1, -1)
);

float CalcOmniShadowFactor(PointLight light, int shadowIndex)
{
    vec3 fragToLight = FragPos - light.position;
    float current = length(fragToLight);
    
    float shadow = 0.0f;
    float bias = 0.05f;
    int samples = 20;
    
    float viewDistance = length(eyePosition - FragPos);
    float diskRadius = (1.0f + (viewDistance / omniShadowMaps[shadowIndex].farPlane )) / 25.0f;
    
    for (int i = 0; i < samples; i++)
    {
        float closest = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closest *= omniShadowMaps[shadowIndex].farPlane;
        if (current - bias > closest)
        {
            shadow += 1.0f;
        }
    }
    
    shadow /= float(samples);
    
    return shadow;
}

float CalcDirectionalShadowFactor(DirectionalLight light)
{
    vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
    projCoords = (projCoords * 0.5) + 0.5;
    
    float current = projCoords.z;
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.direction);
    
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.0005);
    
    float shadow = 0.0f;
    vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
    // PCF (get average shadow)
    if (!(projCoords.z > 1.0))
    {
        float i = 0.0;
        for (int x = -sampleRate; x <= sampleRate; x++)
        {
            for (int y = -sampleRate; y <= sampleRate; y++)
            {
                float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
                shadow += ((current - bias) > pcfDepth) ? 1.0 : 0.0;
                i++;
            }
        }
        
        shadow /= i;
        
    }
    
    return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
    // Ambient lightning
    vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;
    
    // Diffuse lightning
    // A.B = |A||B|cos(angle) = 1*1*cos(angle)
    // Also, make sure we're not below zero (behind)
    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
    vec4 diffuseColour = vec4(light.colour, 1.0f) * light.diffuseIntensity * diffuseFactor;
    
    // Specular lightning
    vec4 specularColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    if (diffuseFactor > 0.0f)
    {
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
        
        float specularFactor = dot(fragToEye, reflectedVertex);
        if (specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
        }
        
    }
    
    return (ambientColour + (1.0 - shadowFactor) * (diffuseColour + specularColour));
}

vec4 CalcDirectionalLight()
{
    float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
    return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight pLight, int shadowIndex)
{
    vec3 direction = FragPos - pLight.position;
    float dist = length(direction);
    direction = normalize(direction);
    
    float shadowFactor = useOmniShadow ? CalcOmniShadowFactor(pLight, shadowIndex) : 0.0f;
    
    vec4 colour = CalcLightByDirection(pLight.base, direction, shadowFactor);
    float attenuation = pLight.exponent * dist * dist +
                        pLight.linear  * dist +
                        pLight.constant;
                        // ax^2 + bx + c
    
    return (colour / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight, int shadowIndex)
{
    vec3 rayDirection = normalize(FragPos - sLight.base.position);
    float slFactor = dot(rayDirection, sLight.direction);
    
    if (slFactor > sLight.edge)
    {
        vec4 colour = CalcPointLight(sLight.base, shadowIndex);
        
        return colour * (1.0f - (1.0f - slFactor) * (1.0f/(1.0f - sLight.edge)));
    } else {
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

vec4 CalcPointLights()
{
    vec4 totalColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < pointLightCount; i++)
    {
        totalColour += CalcPointLight(pointLights[i], i);
    }
    
    return totalColour;
}

vec4 CalcSpotLights()
{
    vec4 totalColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < spotLightCount; i++)
    {
        totalColour += CalcSpotLight(spotLights[i], i + pointLightCount);
    }
    
    return totalColour;
}

void main()
{
    
    vec4 finalColour = CalcDirectionalLight();
    finalColour += CalcPointLights();
    //finalColour += CalcSpotLights();
    colour = texture(theTexture, TexCoord) * finalColour;
     
    
    //vec2 mapCoord = 2.0 * TexCoord - 1.0;
    //colour = texture(omniShadowMaps[1].shadowMap, vec3(1.0, mapCoord.xy));
     
}
