#include "IndexBuffer.h"
#include <cassert>




/**
* Constructor for IndexBuffer Class.
*
*@param[in] unsigned int array pointer.
*@param[in] size in	unsigned int.
*/
IndexBuffer::IndexBuffer(const unsigned int* indices,size_t count) 
	:m_count(count),m_renderBufferId(0)
{
	assert(sizeof(unsigned int) == sizeof(GLuint));// makes sure that both GLuint and unsigned int have same size on the system

	SetupIndexBuffer(indices);// unbinds the index buffer
}

/**
*constructor for IndexBuffer Class.
*
*@param[in] vector of unsigned int.
*@param[in] size in unsigned int.
*/
IndexBuffer::IndexBuffer(const std::vector<unsigned int>& indices, size_t count)
	:m_count(count), m_renderBufferId(0)
{
	unsigned int * indicesArr = new unsigned int [count];
	std::copy(indices.begin(), indices.end(), indicesArr);
	SetupIndexBuffer(indicesArr);// unbinds the index buffer	
}

/**
* Destructor for IndexBuffer Class.
*/
IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_renderBufferId);// deletes the index buffer
}

/**
*Geneartes an index buffer with data from the array.
*
*@param[in] constant unsigned int pointer.
*/
void IndexBuffer::SetupIndexBuffer(const unsigned int* indices)
{
	glGenBuffers(1, &m_renderBufferId);// generates a buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderBufferId);// binds that buffer to the context
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	//passes in the data of the index array

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

/**
*Binds this class instance's index buffer.
*/
void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderBufferId);//binds the index buffer to the current context
}
/**
*Unbinds this class instance's index buffer.
*/
void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//unbinds the index buffer from the current context
}
