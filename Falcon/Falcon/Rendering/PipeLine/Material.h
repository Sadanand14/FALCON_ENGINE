#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "Shader.h"
#include <Types.h>
#include <Memory/fmemory.h>


class Material
{
private:
	std::string m_path;
	std::string m_texPaths[5];
	void BindTo(int32_t location, Texture tex, const char* locName);

public:
	Texture m_albedoTex;
	Texture m_roughnessTex;
	Texture m_normalTex;
	Texture m_metallicTex;
	Texture m_aoTex;

	glm::vec3 m_albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	float m_roughness = 1.0f;
	glm::vec3 m_normal = glm::vec3(0.5f, 0.5, 1.0f);
	float m_metallic = 0.0f;
	float m_ao = 0.0f;
	Shader* m_shader = nullptr;


	void Bind();
	inline void SetShader(Shader* shader) { m_shader = shader; }
	inline const std::string& GetPath() { return m_path; }
	inline void SetPath(const std::string& path) { m_path = path; }
	inline const std::string& GetTexturePath(u32 idx) { return m_texPaths[idx]; }
	inline void SetTexturePath(const std::string& path, u32 idx) { m_texPaths[idx] = path; }
	~Material();
};

#endif