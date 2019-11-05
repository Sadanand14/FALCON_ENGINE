#include "Material.h"

void Material::Bind()
{
	BindTo(0, albedoTex, "albedo");
	BindTo(1, roughnessTex, "roughness");
	BindTo(2, normalTex, "normal");
	BindTo(3, metallicTex, "metal");
	BindTo(4, aoTex, "ao");
}

void Material::BindTo(int32_t location, Texture tex, const char* locName)
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, tex.textureID);
	shader->SetInt(locName, location);
}