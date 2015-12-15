#version 330 core

layout(location = 0) in vec3 vert;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNorm;

out vec2 UV;
out vec3 NORM;
out float DEPTH;

uniform mat4 MVP;

void main()
{
	gl_Position =  MVP * vec4(vert,1);
	UV = vertexUV;
	NORM = vertexNorm;
	DEPTH = gl_Position.z / 2000.0f;
}

