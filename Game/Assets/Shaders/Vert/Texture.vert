#version 330 core

layout(location = 0) in vec3 vert;
layout(location = 1) in vec2 vertexUV;
layout(location = 1) in int tex;

out vec2 UV;
out int TI;

uniform mat4 MVP;

void main()
{
	gl_Position =  MVP * vec4(vert,1);
	UV = vertexUV;
}

