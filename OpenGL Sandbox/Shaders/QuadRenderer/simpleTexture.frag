#version 330 core

out vec4 color;

in vec2 TexCoords;

uniform sampler2D renderedTexture;
uniform float time;

void main()
{
    color = texture( renderedTexture, TexCoords) * vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
