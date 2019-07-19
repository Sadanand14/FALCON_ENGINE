#version 460 core

layout(location = 0) out vec4 FragColor;

in vec2 v_texcoord;

uniform sampler2D texture_diffuse1;

void main()
{
	FragColor = texture(texture_diffuse1, v_texcoord);
}
