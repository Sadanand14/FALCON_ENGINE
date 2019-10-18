#ifndef MATERIAL_H
#define MATERIAL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "Shader.h"

class Material
{
private:
	void BindTo(int32_t location, Texture tex, const char* locName);

public:
	Texture albedoTex;
	Texture roughnessTex;
	Texture normalTex;
	Texture metallicTex;
	Texture aoTex;

	glm::vec3 albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	float roughness = 1.0f;
	glm::vec3 normal = glm::vec3(0.5f, 0.5, 1.0f);
	float metallic = 0.0f;
	float ao = 0.0f;

	Shader* shader;

	void Bind();
};

#endif
