#version 450 core

layout(location = 0) out vec4 FragColor;

in VS_OUT {
	vec2 uv;
	vec4 color;
} fsIn;

uniform sampler2D tex;

void main()
{
	FragColor = fsIn.color * texture(tex, fsIn.uv);
}