#version 450 core

layout (location = 0) out vec4 FragColor;

uniform sampler2D albedo;

in VS_OUT {
	float opacity;
} fsIn;


void main()
{
	FragColor = texture(albedo, gl_PointCoord) * vec4(1.0, 1.0, 1.0, fsIn.opacity);
}
