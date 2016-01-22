#version 330 core

layout(location = 0) in vec3 vert;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNorm;

out vec2 UV;
out vec3 NORM;
out float DEPTH;
out vec3 CAMDIR;

uniform mat4 MVP;
uniform vec3 camDir;

void main()
{
	gl_Position =  MVP * vec4(vert,1);
	UV = vertexUV;
	NORM = vertexNorm;
	DEPTH = gl_Position.z / 1000.0f;
	CAMDIR = camDir;
}

