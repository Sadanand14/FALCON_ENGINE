#version 450 core

layout(location = 0) out vec4 FragColor;

in V_OUT {
	vec3 color;
} fsIn;

void main()
{
	FragColor = vec4(fsIn.color.xyz, 1.0);
}
