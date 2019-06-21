#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "framework.h"

//class to store and initialize a vertex buffer
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

