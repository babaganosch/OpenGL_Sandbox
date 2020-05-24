#version 330 core

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;

out vec4 colour;

struct DirectionalLight
{
    vec3 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

uniform sampler2D theTexture; // GL_TEXTURE0
uniform DirectionalLight directionalLight;

void main()
{
    vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
    
    // A.B = |A||B|cos(angle) = 1*1*cos(angle)
    // Also, make sure we're not below zero (behind)
    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
    
    colour = texture(theTexture, TexCoord) * (ambientColour + diffuseColour);
}
