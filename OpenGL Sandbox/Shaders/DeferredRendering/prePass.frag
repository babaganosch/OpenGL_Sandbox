#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

layout(location = 0) out vec4 diffuse;
layout(location = 1) out vec4 position;
layout(location = 2) out vec4 normal;

uniform sampler2D theTexture;

void main ()
{
    diffuse = texture(theTexture, TexCoord);
    position = vec4(FragPos.xyz, 0.0f);
    normal = vec4(normalize(Normal), 0.0f);
}
