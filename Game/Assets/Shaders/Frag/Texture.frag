#version 330 core

in vec2 UV;
in int TI;

uniform sampler2D TextureSampler;

out vec3 color;

void main()
{
	color = texture( TextureSampler, vec2(UV.x, 1.0 - UV.y) ).rgb;
}