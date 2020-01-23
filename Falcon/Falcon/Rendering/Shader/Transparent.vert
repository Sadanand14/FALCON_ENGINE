#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in mat4 model;

out V_OUT {
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
} vsOut;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vsOut.uv = uv;
	vsOut.normal = normal;
	vsOut.tangent = tangent;
	vsOut.bitangent = bitangent;

	gl_Position = projection * view * model * vec4(position, 1.0);
}