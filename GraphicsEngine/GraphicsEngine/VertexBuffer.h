#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

class VertexBuffer
{
private:
	unsigned int renderBufferId;
public:
	VertexBuffer(const void*, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

#endif // !1

