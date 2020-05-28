#version 330 core

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

in float life;

layout(location = 0) out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(0.f, 1.f, 0.f, 1.f);//vec4(material_color, 1.0);
}
