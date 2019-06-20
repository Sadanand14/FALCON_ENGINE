#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <glad/glad.h>

class IndexBuffer
{
private:
	unsigned int renderBufferId;
	unsigned int m_count;
public:

	IndexBuffer(const unsigned int*, unsigned int);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
};

#endif // !1
