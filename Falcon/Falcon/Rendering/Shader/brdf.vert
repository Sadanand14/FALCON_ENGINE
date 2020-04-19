#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in mat4 model;
layout (location = 9) in mat3 normalMat;

out VS_OUT
{
	vec2 uv;
} vsOut;

void main()
{
	gl_Position = vec4(position, 1.0);
	vsOut.uv = uv;
}
