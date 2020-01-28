#version 450 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec2 i_UV;
layout(location = 2) in vec3 i_Normal;
layout(location = 3) in vec3 i_Tangent;
layout(location = 4) in vec3 i_BiTangent;
layout(location = 5) in mat4 i_model;

uniform mat4 view;
uniform mat4 proj;

out VS_OUT
{
	vec3 cubeDirection;
}vsOut;

void main()
{
	mat4 vp = proj * mat4(mat3(view));
	gl_Position = vp*vec4(i_Pos, 1.0f).xyww ;

	vsOut.cubeDirection = i_Pos;
}