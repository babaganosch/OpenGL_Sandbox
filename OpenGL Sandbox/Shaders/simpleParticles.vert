#version 330 core

layout(location = 0) in vec4 particle;
uniform mat4 projection;

out float life; 

void main()
{
	life = particle.w;
	gl_Position = projection * vec4(particle.xyz, 1.0);
}
