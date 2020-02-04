#version 450 core

layout(location=0) in vec3 i_Pos;
layout(location=1) in vec2 i_UV;
layout(location=2) in vec3 i_normal; 
layout(location=3) in vec3 i_tangent;
layout(location=4) in vec3 i_biTangent;
layout(location=5) in mat4 i_model;

uniform mat4 view;
uniform mat4 projection;

