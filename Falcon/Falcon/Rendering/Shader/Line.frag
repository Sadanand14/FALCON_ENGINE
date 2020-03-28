#version 450 core

layout (location = 0) out vec4 FragColor;

in VS_OUT {
	vec3 color;
} fsIn;


void main()
{
	FragColor = vec4(fsIn.color, 1.0);
}
