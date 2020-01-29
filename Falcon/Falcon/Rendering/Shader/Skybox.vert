#version 450 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec2 i_UV;
layout(location = 2) in vec3 i_Normal;
layout(location = 3) in vec3 i_Tangent;
layout(location = 4) in vec3 i_BiTangent;
layout(location = 5) in mat4 i_model;

uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
	vec3 cubeDirection;
}vsOut;

void main()
{
	mat4 newView = view;
	newView[3][0] = 0;
	newView[3][1] = 0;
	newView[3][2] = 0;
	mat4 vp = projection * mat4(mat3(view));
	gl_Position = (vp*vec4(i_Pos, 1.0f)).xyww ;

	vsOut.cubeDirection = i_Pos;
}