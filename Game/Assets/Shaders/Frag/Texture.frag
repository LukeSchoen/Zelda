#version 330 core

in vec2 UV;
in vec3 NORM;
in float DEPTH;

uniform sampler2D TextureSampler;

out vec3 color;

void main()
{
	color = texture( TextureSampler, vec2(UV.x, 1.0 - UV.y) ).rgb;
	float amp = abs(NORM.y);
	amp = (amp * amp * amp * amp * amp * amp);
	amp = max(amp * 1.25,0.75);
	color *= amp;

	if (DEPTH < 1.0)
	{
		float NDEPTH = max(DEPTH,0.4);
		vec3 d = vec3(NDEPTH ,NDEPTH ,NDEPTH );
		vec3 detail = texture( TextureSampler, vec2(UV.x * 5, (1.0 - UV.y) * 5) ).rgb;
		color = color * d + detail * (vec3(1.0,1.0,1.0) - d);
	}

}