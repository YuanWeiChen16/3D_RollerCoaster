#version 430 core

layout (location = 0 )in vec3 vertics;
layout (location = 1 )in vec2 UV;


out vec2 poiz;

void main()
{
	gl_Position = vec4(vertics.x,vertics.y,0.0,1.0);
	poiz=UV;
}