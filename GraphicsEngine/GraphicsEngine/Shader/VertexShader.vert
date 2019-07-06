#SHADER VERTEX
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texcoord;

out vec2 v_texcoord;

void main() 
{
	v_texcoord = texcoord;
	gl_Position = position;
}