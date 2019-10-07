#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "framework.h"

/**
* A Vertex Buffer Class to initialize and store Vertex Data into a buffer using OpenGL commands..
*/
class VertexBuffer
{
private:
	unsigned int m_renderBufferId;
public:
	VertexBuffer(const void*,size_t size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

#endif // !1

