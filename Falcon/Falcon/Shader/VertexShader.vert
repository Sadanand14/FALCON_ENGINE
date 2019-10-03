#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

out vec2 v_texcoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() 
{
	v_texcoord = texcoord;
	gl_Position = projection * view * model * vec4(position, 1.0);
}
