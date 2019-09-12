#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "framework.h"
#include <vector>

/**
* Index Buffer CLass to initialize an index buffer and store data on using OpenGL commands.
*/
class IndexBuffer
{
private:
	unsigned int m_renderBufferId;
	size_t m_count;
	void SetupIndexBuffer(const unsigned int* indices);

public:

	IndexBuffer(const unsigned int*, size_t);
	IndexBuffer(const std::vector<unsigned int>& indices, size_t count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
};

#endif // !1