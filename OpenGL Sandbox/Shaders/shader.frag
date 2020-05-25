#version 330 core

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;

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

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D theTexture; // GL_TEXTURE0
uniform Material material;

uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 direction)
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
    
    return (ambientColour + diffuseColour + specularColour);
}

vec4 CalcDirectionalLight()
{
    return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLights()
{
    vec4 totalColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < pointLightCount; i++)
    {
        vec3 direction = FragPos - pointLights[i].position;
        float dist = length(direction);
        direction = normalize(direction);
        
        vec4 colour = CalcLightByDirection(pointLights[i].base, direction);
        float attenuation = pointLights[i].exponent * dist * dist +
                            pointLights[i].linear  * dist +
                            pointLights[i].constant;
                            // ax^2 + bx + c
        
        totalColour += (colour / attenuation);
    }
    
    return totalColour;
}

void main()
{
    vec4 finalColour = CalcDirectionalLight();
    finalColour += CalcPointLights();
    colour = texture(theTexture, TexCoord) * finalColour;
}
