#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "framework.h"

#include <string>

class Texture 
{
private:
	unsigned int m_textureID;
	int	m_width, m_height, m_BPP;
	std::string m_filepath;
	unsigned char* m_localBuffer;

public:
	Texture(std::string);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline unsigned int GetHeight() { return m_height; }
	inline unsigned int GetWidth() { return m_width; }
	inline unsigned int GetID() { return m_textureID; }
	inline std::string  GetPath() { return m_filepath; }
};

#endif // !1
