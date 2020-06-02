#version 330 core

in vec2 TexCoords;
out vec4 outColour;

uniform sampler2D renderedTexture;
uniform sampler2D depthTexture;

void main()
{
    outColour = texture(renderedTexture, TexCoords);
    return;
}
