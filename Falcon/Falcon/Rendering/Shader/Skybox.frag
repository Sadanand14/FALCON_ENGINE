#version 450 core

layout (location = 0  ) out vec4 Color;

uniform samplerCube skyTexture;

in VS_OUT
{
	vec3 cubeDirection;
}fsIn;

void main()
{
	Color = texture(skyTexture, fsIn.cubeDirection);
}