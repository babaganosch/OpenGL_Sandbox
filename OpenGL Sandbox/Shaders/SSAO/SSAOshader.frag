#version 330 core

#define radius 0.5f
#define kernelSize 16

in vec2 TexCoords;
out float FragColor;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D noiseTexture;

uniform vec3 samples[kernelSize];
uniform mat4 projection;
uniform mat4 projectionView;
uniform float s_width;
uniform float s_height;

uniform vec3 cameraPosition;
uniform vec3 cameraDirection;


void main()
{
    vec2 noiseScale = vec2(s_width/4.0, s_height/4.0);
    
    vec3 fragPos   = texture(positionTexture, TexCoords).xyz;
    vec3 normal    = texture(normalTexture, TexCoords).rgb;
    vec3 randomVec = texture(noiseTexture, TexCoords * noiseScale).xyz;
    
    vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN       = mat3(tangent, bitangent, normal);
    
    float bias = 0.0001;
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 Sample = TBN * samples[i]; // From tangent to view-space
        Sample = fragPos + Sample * radius;

        float SampleZ = dot(Sample - cameraPosition, cameraDirection); // cameraDirection must be normalized!

        vec4 offset = vec4(Sample, 1.0);
        offset = projectionView * offset; // from world to clip-space NOTE: we use projection * view instead of view
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

        vec3 toOriginal = texture(positionTexture, offset.xy).xyz - cameraPosition;
        float sampleDepth = dot(cameraDirection, toOriginal);

        occlusion += (sampleDepth >= SampleZ + bias ? 1.0 : 0.0);
        /*
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion       += (sampleDepth >= SampleZ + bias ? 1.0 : 0.0) * rangeCheck;
         */
    }
    
    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = occlusion;
}
