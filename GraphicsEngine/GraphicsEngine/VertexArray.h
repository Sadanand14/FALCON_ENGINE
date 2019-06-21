#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "VertexLayout.h"

//class to initialize and store vertex array object its bound vertexbuffer and its attributes
class VertexArray
{
private:
	unsigned int m_rendererID;

public:

	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer&, const VertexLayout&);
};

#endif // !1
