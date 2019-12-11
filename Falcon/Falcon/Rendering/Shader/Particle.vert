#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in float size;
layout (location = 2) in float opacity;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 camPos;

out VS_OUT {
	float opacity;
} vsOut;

void main()
{
	vec4 mvPos = view * model * vec4(pos, 1.0);

	float dist = distance(mvPos.xyz, camPos);
	gl_PointSize = size * 200.0 / dist;
	gl_Position = projection * mvPos;
	vsOut.opacity = opacity;
}