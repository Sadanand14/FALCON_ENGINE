#include "Texture.h"
#include "stb_image.h"


Texture::Texture(boost::container::string path): m_filepath(path)
{
	stbi_set_flip_vertically_on_load(1);
	m_localBuffer = stbi_load(m_filepath.c_str(), &m_width, &m_height, &m_BPP , 4);

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D,m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_INT, m_localBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_localBuffer) 
	{
		stbi_image_free(m_localBuffer);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1,&m_textureID);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

