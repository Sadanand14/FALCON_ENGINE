#include "IndexBuffer.h"
#include <cassert>

IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count) :m_count(count)
{
	assert(sizeof(unsigned int) == sizeof(GLuint));

	glGenBuffers(1, &renderBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &renderBufferId);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderBufferId);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
