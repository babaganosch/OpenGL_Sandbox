#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    TexCoord = tex;
    
    // Only use the 3x3 part of the model matrix, and don't get skewed due to scaling (transpose and inverse)
    Normal = mat3(transpose(inverse(model))) * norm;
    
    FragPos = (model * vec4(pos, 1.0f)).xyz;
}
