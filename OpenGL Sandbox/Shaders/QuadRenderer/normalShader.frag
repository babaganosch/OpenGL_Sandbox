#version 330 core

out vec4 color;

in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D renderedTexture;

void main()
{
    color = normalize(Normal);
}
