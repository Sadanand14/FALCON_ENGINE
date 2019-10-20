#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "VertexLayout.h"

/**
*Class to initialize and store vertex array object its bound vertexbuffer and its attributes using OpenGL commands..
*/
class VertexArray
{
private:
	unsigned int m_rendererID;
public:

	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexAttribPointer(u32 loc, u32 size, u32 type, u32 normalize, u32 byteSize, u64 offset, u32 divisor);
};

#endif // !1
