#SHADER FRAGMENT
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_texcoord;

uniform sampler2D u_color;

void main()
{
	vec4 tex = texture(u_color, v_texcoord);
	color = tex;
}