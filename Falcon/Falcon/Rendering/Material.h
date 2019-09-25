#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

class Material
{
//private:
public:
	Texture* albedoTex = nullptr;
	Texture* roughnessTex = nullptr;
	Texture* normalTex = nullptr;
	Texture* metallicTex = nullptr;
	Texture* aoTex = nullptr;
	
	glm::vec3 albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	float roughness = 1.0f;
	glm::vec3 normal = glm::vec3(0.5f, 0.5, 1.0f);
	float metal = 0.0f;
	float aoTex = 0.0f;
	
	Shader* shader;
//public:
};

#endif
