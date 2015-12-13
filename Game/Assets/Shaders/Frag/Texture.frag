#version 330 core

in vec2 UV;
in vec3 NORM;

uniform sampler2D TextureSampler;

out vec3 color;

void main()
{
	color = texture( TextureSampler, vec2(UV.x, 1.0 - UV.y) ).rgb;
	//color*= 0.75 + abs(NORM.y)*0.25;
}