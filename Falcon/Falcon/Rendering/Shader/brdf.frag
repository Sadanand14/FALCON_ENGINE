#version 450 core

in VS_OUT
{
	vec2 uv;
} fsIn;

out vec2 fragColor;

const float PI = 3.14159265359;
const uint SAMPLE_COUNT = 1024u;
const float ONE_OVER_SAMPLE_COUNT = 0.0009765625;

float radicalInverse(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 hammersley(uint i, uint normal)
{
	return vec2(float(i) / float(normal), radicalInverse(i));
}

float geometryGGX(float nDotV, float nDotL, float roughness)
{
	float k = (roughness * roughness) * 0.5;

	float geomNV = nDotV / (nDotV * (1.0 - k) + k);
	float geomNL = nDotL / (nDotL * (1.0 - k) + k);

	return geomNV * geomNL;
}

vec3 importanceSample(vec2 xi, vec3 normal, float roughness)
{
    float a = roughness * roughness;

    float phi = 2.0 * PI * xi.x;
    float cosTheta = sqrt((1.0 - xi.y) / (1.0 + (a * a - 1.0) * xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    vec3 h = vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);

    // from tangent-space vector to world-space sample vector
    vec3 up = abs(normal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, normal));
    vec3 bitangent = cross(normal, tangent);

    vec3 sampleVec = tangent * h.x + bitangent * h.y + normal * h.z;
    return normalize(sampleVec);
}

vec2 integrateBRDF(float nDotV, float roughness)
{
	vec3 v = vec3(sqrt(1.0 - nDotV * nDotV), 0.0, nDotV);
	vec3 norm = vec3(0.0, 0.0, 1.0);
	float a = 0.0;
	float b = 0.0;

	for(uint i = 0u; i < SAMPLE_COUNT; i++) {
		vec2 xi = hammersley(i, SAMPLE_COUNT);
		vec3 h = importanceSample(xi, norm, roughness);
		vec3 l = normalize(2.0 * dot(v, h) * h - v);

		float nDotL = max(l.z, 0.0);

		if (nDotL > 0.0) {
			float nDotH = max(h.z, 0.0);
			float vDotH = max(dot(v, h), 0.0);
			float g = geometryGGX(nDotV, nDotL, roughness);
			float gVis = (g * vDotH) / (nDotH * nDotV);
			float fc = pow(1.0 - vDotH, 5.0);

			a += (1.0 - fc) * gVis;
			b += fc * gVis;
		}
	}

	a *= ONE_OVER_SAMPLE_COUNT;
	b *= ONE_OVER_SAMPLE_COUNT;

	return vec2(a, b);
}

void main()
{
	vec2 integratedBRDF = integrateBRDF(fsIn.uv.x, fsIn.uv.y);
	fragColor = integratedBRDF;
}
