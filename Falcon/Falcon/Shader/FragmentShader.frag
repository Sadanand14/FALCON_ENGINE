#version 460 core

layout(location = 0) out vec4 FragColor;

in vec2 v_texcoord;

uniform sampler2D albedo;

void main()
{
	FragColor = texture(albedo, v_texcoord);
}
