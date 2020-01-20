#include "Material.h"

/**
 * Binds a material for rendering
 */
void Material::Bind()
{
	BindTo(0, m_albedoTex, "albedo");
	BindTo(1, m_roughnessTex, "roughness");
	BindTo(2, m_normalTex, "normal");
	BindTo(3, m_metallicTex, "metal");
	BindTo(4, m_aoTex, "ao");
}

/**
 * Binds a texture to a location in the shader
 */
void Material::BindTo(int32_t location, Texture tex, const char* locName)
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, tex.textureID);
	m_shader->SetInt(locName, location);
}

Material::~Material()
{
	fmemory::fdelete<>(m_shader);
	glDeleteTextures(1, &m_albedoTex.textureID);
	glDeleteTextures(1, &m_roughnessTex.textureID);
	glDeleteTextures(1, &m_normalTex.textureID);
	glDeleteTextures(1, &m_metallicTex.textureID);
	glDeleteTextures(1, &m_aoTex.textureID);
}