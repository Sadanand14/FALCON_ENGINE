#version 450 core

layout (location = 0) in mat4 model;

out V_OUT {
	vec3 color;
} vsOut;

uniform mat4 projection;
uniform mat4 view;
uniform vec4 color;

void main()
{
	vsOut.color = color.xyz;
	gl_Position = projection * view * model * vec4(0.0, 0.0, 0.0, 1.0);
	gl_PointSize = color.w;
}