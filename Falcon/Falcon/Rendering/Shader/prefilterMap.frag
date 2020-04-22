#version 450 core

out vec4 fragColor;
in vec3 localPos;

uniform samplerCube cubeMap;
uniform vec2 res;
uniform float roughness;

const float PI = 3.14159265359;
const uint SAMPLE_COUNT = 2048u;
const float INVERSE_NUM_SAMPLES = 1.0 / float(SAMPLE_COUNT);

float radicalInverse(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10;
}

vec2 hammersley(uint i)
{
	return vec2(float(i) * INVERSE_NUM_SAMPLES, radicalInverse(i));
}

float distributionGGX(float nDotH, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;

	float nDotH2 = nDotH * nDotH;

	float denom = nDotH2 * (a2 - 1.0) + 1.0;
	denom = PI * denom * denom;

	return a2 / denom;
}

vec3 importanceSample(vec2 xi, vec3 normal, float roughness)
{
	float a = roughness * roughness;

	float phi = 2.0 * PI * xi.x;
	float cosTheta = sqrt((1.0 - xi.y) / (1.0 + (a * a - 1.0) * xi.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	vec3 h = vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);

	vec3 up = abs(normal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, normal));
	vec3 bitangent = cross(normal, tangent);

	return normalize(tangent * h.x + bitangent * h.y + normal * h.z);
}


void main()
{
	vec3 norm = normalize(localPos);
	vec3 v = norm;

	vec3 prefilteredColor = vec3(0.0);
	float totalWeight = 0.0;

	for(uint i = 0u; i < SAMPLE_COUNT; i++) {
		vec2 xi = hammersley(i);
		vec3 h = importanceSample(xi, norm, roughness);
		float vDotH = dot(v, h);
		vec3 l = normalize(2.0 * vDotH * h - v);
		vDotH = max(vDotH, 0.0);

		float nDotL = max(dot(norm, l), 0.0);

		if (nDotL > 0.0) {
			float nDotH = max(dot(norm, h), 0.0);
			float d = distributionGGX(nDotH, roughness);
			float pdf = (d * nDotH / (4.0 * vDotH)) + 0.0001;

			float saTex = 4.0 * PI / (6 * res.x * res.y);
			float saSamp = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);
			float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSamp / saTex);

			prefilteredColor += textureLod(cubeMap, l, mipLevel).rgb * nDotL;
			totalWeight += nDotL;
		}
    }

	prefilteredColor /= totalWeight;

	fragColor = vec4(prefilteredColor, 1.0);
}