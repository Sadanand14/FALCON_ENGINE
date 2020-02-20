#version 450 core

layout (location = 0  ) out vec4 Color;
in vec2 out_UV;

uniform sampler2D quadTexture;

void main()
{
	Color = texture(quadTexture, out_UV);
}
