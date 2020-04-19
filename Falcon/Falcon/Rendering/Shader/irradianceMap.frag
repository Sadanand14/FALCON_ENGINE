#version 450 core

out vec4 fragColor;
in vec3 localPos;

uniform samplerCube cubeMap;

const float PI = 3.14159265359;

void main()
{
	vec3 norm = normalize(localPos);
	vec3 irradiance = vec3(0.0);

	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(up, norm));
	up = cross(norm, right);

	int numSamples = 0;

	for(float i = 0.0; i < 2.0 * PI; i += 0.025) {
		for(float j = 0.0; j < 0.5 * PI; j += 0.025) {
			vec3 tanSample = vec3(sin(j) * cos(i), sin(j) * sin(i), cos(j));
			vec3 sampleVec = tanSample.x * right + tanSample.y * up + tanSample.z * norm;

			irradiance += texture(cubeMap, sampleVec).rgb * cos(j) * sin(j);
			numSamples++;
		}
	}

	irradiance = PI * irradiance / float(numSamples);

	fragColor = vec4(irradiance, 1.0);
}