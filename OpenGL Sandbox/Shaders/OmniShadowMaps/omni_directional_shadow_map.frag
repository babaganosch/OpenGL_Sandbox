#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
	float dist = length(FragPos.xyz - lightPos);
	dist = dist/farPlane;
	gl_FragDepth = dist;
}
