#version 450 core

layout(location = 0) out vec4 FragColor;

in V_OUT {
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
} fsIn;

uniform sampler2D albedo;

void main()
{
	vec3 svd = fsIn.normal * fsIn.tangent * fsIn.bitangent;
	FragColor = texture(albedo, fsIn.uv).rgba;
}
