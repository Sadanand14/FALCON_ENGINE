#version 450 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 color;

out VS_OUT {
	vec2 uv;
	vec4 color;
} vsOut;

uniform mat4 proj;

void main()
{
	vsOut.uv = uv;
	vsOut.color = color;
	gl_Position = proj * vec4(pos.xy, 0, 1);
}