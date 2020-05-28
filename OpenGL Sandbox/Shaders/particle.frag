#version 330 core
in float life;

layout(location = 0) out vec4 Colour;

uniform sampler2D theTexture;

void main()
{
	// Base color.
	Colour = texture(theTexture, gl_PointCoord);
	// Make it darker the older it is.
	Colour.xyz *= (1.0 - life);
	// Make it fade out the older it is, also all particles have a
	// very low base alpha so they blend together.
	Colour.w = Colour.w * (1.0 - pow(life, 4.0)) * 0.05;
}
