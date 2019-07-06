#include "IndexBuffer.h"
#include <cassert>

//creates and binds an index buffer 
IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count) :m_count(count),m_renderBufferId(0)
{
	assert(sizeof(unsigned int) == sizeof(GLuint));// makes sure that both GLuint and unsigned int have same size on the system

	glGenBuffers(1, &m_renderBufferId);// generates a buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderBufferId);// binds that buffer to the context
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	//passes in the data of the index array

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);// unbinds the index buffer
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_renderBufferId);// deletes the index buffer
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderBufferId);//binds the index buffer to the current context
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//unbinds the index buffer from the current context
}
