#version 330 core

#define pi 3.141592653589793238462643383279

in vec2 UV;
in vec3 NORM;
in float DEPTH;
in vec3 CAMDIR;

uniform sampler2D TextureSampler;

out vec3 color;

void main()
{
	vec3 c = texture( TextureSampler, vec2(UV.x, 1.0 - UV.y) ).rgb;
	//vec3 c = vec3(1.0, 1.0, 1.0);
	float multiplier = asin(abs(NORM.y));
	multiplier = multiplier / (pi / 2.0);
	color = c * multiplier;

	multiplier = abs(dot(NORM, CAMDIR));
	multiplier = asin(multiplier);
	multiplier = multiplier / (pi / 2.0);
	color += c * multiplier;

	color = color / 1.5;
	//color = c;
}