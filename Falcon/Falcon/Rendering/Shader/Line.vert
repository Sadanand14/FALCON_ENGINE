#version 450 core

layout (location = 0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 color;

out VS_OUT {
	vec3 color;
} vsOut;

void main()
{
	gl_Position = projection * view * vec4(pos, 1.0);
	vsOut.color = color;
}