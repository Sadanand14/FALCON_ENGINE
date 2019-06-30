#ifndef TEXTURE_H
#define TEXTURE_H

#include "framework.h"

#include <boost/container/string.hpp>
#include <string>

class Texture 
{
private:
	unsigned int m_textureID;
	int	m_width, m_height, m_BPP;
	boost::container::string m_filepath;
	unsigned char* m_localBuffer;

public:
	Texture(boost::container::string);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline unsigned int GetHeight() { return m_height; }
	inline unsigned int GetWidth() { return m_width; }
	inline unsigned int GetID() { return m_textureID; }
	inline boost::container::string GetPath() { return m_filepath; }
};

#endif // !1
