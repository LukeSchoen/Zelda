#version 330 core

in vec2 UV;
in vec3 NORM;
in float DEPTH;
in vec3 CAMDIR;

uniform sampler2D TextureSampler;

out vec3 color;

vec3 saturation(vec3 rgb, float adjustment)
{
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
    vec3 intensity = vec3(dot(rgb, W));
    return mix(intensity, rgb, adjustment);
}

void main()
{

	vec3 c = texture( TextureSampler, vec2(UV.x, 1.0 - UV.y) ).rgb;
	
	//Take out of SRGB
	c = c * c;
	
	//Sky light
	float skyLight = NORM.y;

	//Camera light
	float camLight = dot(NORM, CAMDIR);
	
	
	camLight = max(camLight,0);

	skyLight = skyLight * skyLight * skyLight * skyLight * skyLight;
	skyLight = skyLight * skyLight * skyLight * skyLight * skyLight;
	skyLight *= camLight+0.25;

	camLight = camLight * camLight * camLight * camLight;
	
	//float light = max(skyLight, camLight);
	float light = skyLight + (0.4+camLight);

	light = min(light , 1);
	
	//Put into SRGB
	c = sqrt(c * light);
	
	color = c;

	color = saturation(color, 1.75);

}