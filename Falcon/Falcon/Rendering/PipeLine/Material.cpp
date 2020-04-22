#include "Material.h"

/**
 * Binds a material for rendering
 */
void Material::Bind()
{
	BindTo(0, m_albedoTex, "albedoTex");
	BindTo(1, m_normalTex, "normalTex");
	BindTo(2, m_roughnessTex, "roughnessTex");
	BindTo(3, m_metallicTex, "metallicTex");
	BindTo(4, m_aoTex, "aoTex");
}

/**
 * Binds a texture to a location in the shader
 */
void Material::BindTo(int32_t location, Texture tex, const char* locName)
{
	glActiveTexture(GL_TEXTURE0 + location);

	if (tex.type == TextureType::CUBEMAP)
		glBindTexture(GL_TEXTURE_CUBE_MAP, tex.textureID);
	else if(tex.type == TextureType::TEXTURE2D)
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