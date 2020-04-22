#version 450 core

#define PI 3.14159265359
#define ONE_OVER_PI 0.318309886184
#define MAX_REFLECTION_LOD 4.0
const vec3 F0_NON_METAL = vec3(0.04);

layout(location = 0) out vec4 FragColor;

in V_OUT {
	vec3 pos;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	mat3 normalMat;
} fsIn;

uniform vec3 camPos;
uniform sampler2D albedoTex;
uniform sampler2D normalTex;
uniform sampler2D roughnessTex;
uniform sampler2D metallicTex;
uniform sampler2D aoTex;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdf;

struct Light
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	Light light;
	vec3 dir;
};

struct PointLight
{
	Light light;
	vec3 pos;
	float radius;
};

//Lighting equations
vec3 fresnelSchlick(float cosTheta, vec3 f0)
{
	return f0 + (1.0 - f0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 f0, float roughness)
{
    return f0 + (max(vec3(1.0 - roughness), f0) - f0) * pow(1.0 - cosTheta, 5.0);
}

float distributionGGX(vec3 n, vec3 h, float roughness)
{
	float a = roughness * roughness;
	float a2 = max(a * a, 0.0000001);

	float nDotH = max(dot(n, h), 0.0);
	float nDotH2 = nDotH * nDotH;

	float denom = nDotH2 * (a2 - 1.0) + 1.0;
	denom = PI * denom * denom;

	return a2 / denom;
}

float partialGeometryGGX(float nDotV, float k)
{
	return nDotV / (nDotV * (1.0 - k) + k);
}

float geometryGGX(float nDotV, float nDotL, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) * 0.125;

	return partialGeometryGGX(nDotV, k) * partialGeometryGGX(nDotL, k);
}

vec3 getBasicLighting(vec3 dir, vec3 pos, vec3 v, vec3 f0, vec3 albedo, vec3 normal, float roughness, float metallic)
{
	//Lighting variables
	vec3 l = normalize(dir); //Normalized light dir
	vec3 h = normalize(l + v); //Half vector
	vec3 r = reflect(-v, normal); //Reflection of -v over half vector
	float nDotV = max(dot(normal, v), 0.0);
	float nDotL = max(dot(normal, l), 0.0); // Lambert diffuse

	float ndf = distributionGGX(normal, h, roughness);
	float g = geometryGGX(nDotV, nDotL, roughness);

	vec3 ks = fresnelSchlick(max(dot(v, h), 0.0), f0); //Fresnel factor for spec on edges

	vec3 num = ndf * ks * g;
	float denom = 4.0 * max(nDotV, nDotL) + 0.0001;
	vec3 spec = num / denom; //Specular

	vec3 kd = vec3(1.0) - ks;
	kd *= 1.0 - metallic;

	return (kd * nDotL * albedo + spec);
}

vec3 calcDirectionalLight(DirectionalLight dl, vec3 pos, vec3 v, vec3 f0, vec3 albedo, vec3 normal, float roughness, float metallic)
{
	//Attenuation
	float att = dl.light.intensity;
	vec3 rad = dl.light.color * att;

	//Get lighting, scale by attenuation
	vec3 color = getBasicLighting(dl.dir, pos, v, f0, albedo, normal, roughness, metallic) * rad;
	return color;
}

vec3 calcPointLight(PointLight pl, vec3 pos, vec3 v, vec3 f0, vec3 albedo, vec3 normal, float roughness, float metallic)
{
	////Lighting variables
	vec3 dir = pl.pos - pos;
	float dist = length(dir);

	//Attenuation
	float att = clamp(1.0 - dist * dist / (pl.radius * pl.radius), 0.0, 1.0);
	att *= att;
	vec3 rad = pl.light.color * att * pl.light.intensity;

	//Get lighting, scale by attenuation
	vec3 color = getBasicLighting(dir, pos, v, f0, albedo, normal, roughness, metallic) * rad;
	return color; //Return light color
}

void main()
{
	vec3 n = fsIn.normalMat * fsIn.normal;
	vec3 t = fsIn.normalMat * fsIn.tangent;
	t = normalize(t - dot(t, n) * n);
	vec3 b = fsIn.normalMat * fsIn.tangent;
	b = normalize(b - dot(b, n) * n - dot(b, t) * t);

	mat3 tbn = mat3(t, b, n);

	vec4 albedo = texture(albedoTex, fsIn.uv);
	vec3 normal = normalize(texture(normalTex, fsIn.uv).rgb * 2.0 - 1.0);
	normal = normalize(tbn * normal);
	float roughness = texture(roughnessTex, fsIn.uv).r;
	float metallic = texture(metallicTex, fsIn.uv).r;
	float ao = texture(aoTex, fsIn.uv).r;

	vec3 v = normalize(camPos - fsIn.pos); //Normalized angle from view pos to obj pos

	DirectionalLight dl;
	dl.dir = vec3(0.0, 0.0, -1.0);
	dl.light.color = vec3(1.0, 1.0, 1.0);
	dl.light.intensity = 1.0;

	vec3 f0 = mix(F0_NON_METAL, albedo.rgb, metallic);
	vec3 lightColor = calcDirectionalLight(dl, fsIn.pos, v, f0, albedo.rgb, normal, roughness, metallic);

	vec3 r = reflect(-v, normal);
	float nDotV = max(dot(normal, v), 0.0);

	//Ambient lighting
	vec3 ks = fresnelSchlickRoughness(nDotV, f0, roughness);
	vec3 kd = 1.0 - ks;
	kd *= 1.0 - metallic;

	vec3 prefilteredColor = textureLod(prefilterMap, r, roughness * MAX_REFLECTION_LOD).rgb;
	vec2 envBrdf = texture(brdf, vec2(nDotV, roughness)).rg;
	vec3 spec = prefilteredColor * (ks * envBrdf.x + envBrdf.y);

	vec3 irradiance = texture(irradianceMap, normal).rgb;
	vec3 diff = irradiance * albedo.rgb;
	vec3 ambient = (kd * diff + spec) * ao;

	vec3 color = lightColor + ambient;
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));
	FragColor = vec4(color, albedo.a);
}
