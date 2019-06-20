#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* databuffer, unsigned int size)
{
	glGenBuffers(1, &renderBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, renderBufferId);
	glBufferData(GL_ARRAY_BUFFER, size, databuffer, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &renderBufferId);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, renderBufferId);
}


void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

