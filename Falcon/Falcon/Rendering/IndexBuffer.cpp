#include <cassert>
#include "IndexBuffer.h"
#include "Memory/fmemory.h"

//creates and binds an index buffer with int array
IndexBuffer::IndexBuffer(const unsigned int* indices,size_t count) 
	:m_count(count),m_renderBufferId(0)
{
	assert(sizeof(unsigned int) == sizeof(GLuint));// makes sure that both GLuint and unsigned int have same size on the system

	SetupIndexBuffer(indices);// unbinds the index buffer
}



//creates and binds an index buffer with int vector
IndexBuffer::IndexBuffer(const std::vector<unsigned int,fmemory::STLAllocator<unsigned int>>& indices, size_t count)
	:m_count(count), m_renderBufferId(0)
{
	unsigned int* indicesArr = fmemory::fnew <unsigned int,count>(0);// new unsigned int[count];

	std::copy(indices.begin(), indices.end(), indicesArr);
	SetupIndexBuffer(indicesArr);// unbinds the index buffer	

	fmemory::fdelete <unsigned int, count>(indicesArr);
}



//Geneartes and bind data to the index buffer
void IndexBuffer::SetupIndexBuffer(const unsigned int* indices)
{
	glGenBuffers(1, &m_renderBufferId);// generates a buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderBufferId);// binds that buffer to the context
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	//passes in the data of the index array

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
