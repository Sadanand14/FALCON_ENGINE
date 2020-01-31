#version 450 core

layout (location = 0) in vec3 i_Pos;
layout (location = 1) in vec2 i_UV;
layout (location = 2) in vec3 i_normal;
layout (location = 3) in vec3 i_Tangent;
layout (location = 4) in vec3 i_Bitangent;
layout (location = 5) in mat4 i_model;

out vec3 localPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    localPos = iPos;  
    gl_Position =  projection * view * vec4(localPos, 1.0);
}