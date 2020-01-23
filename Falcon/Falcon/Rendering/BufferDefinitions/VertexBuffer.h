#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "framework.h"
#include <Types.h>

/**
* A Vertex Buffer Class to initialize and store Vertex Data into a buffer using OpenGL commands..
*/
class VertexBuffer
{
private:
	u32 m_renderBufferId;

public:
	VertexBuffer(const void*,size_t size, u32 drawType);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void BufferData(const void* data, size_t size, u32 drawType);
	void BufferSubData(const void* data, uint32_t offset, size_t size);
};

#endif // !1

