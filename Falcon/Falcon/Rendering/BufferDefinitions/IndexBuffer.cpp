#include "IndexBuffer.h"
#include <cassert>
#include "Memory/fmemory.h"

boost::mutex IndexBuffer::m_indexMtx;

/**
* Constructor for IndexBuffer Class.
*
*@param[in] unsigned int array pointer.
*@param[in] size in	unsigned int.
*/
IndexBuffer::IndexBuffer(const unsigned int* indices,size_t count, u32 drawType)
	:m_count(count),m_renderBufferId(0)
{
	assert(sizeof(unsigned int) == sizeof(GLuint));// makes sure that both GLuint and unsigned int have same size on the system

	SetupIndexBuffer(indices, drawType);// unbinds the index buffer
}

/**
 * Constructor for IndexBuffer Class.
 *
 *@param[in] unsigned int array pointer.
 *@param[in] size in	unsigned int.
 */
IndexBuffer::IndexBuffer(const unsigned int* indices, size_t count)
: IndexBuffer(indices, count, GL_STATIC_DRAW)
{

}

/**
*constructor for IndexBuffer Class.
*
*@param[in] vector of unsigned int.
*@param[in] size in unsigned int.
*/
IndexBuffer::IndexBuffer(const std::vector<unsigned int,fmemory::StackSTLAllocator<unsigned int>>& indices, size_t count = 1, u32 drawType = GL_STATIC_DRAW)
	:m_count(count), m_renderBufferId(0)
{
	SetupIndexBuffer(indices.data(), drawType);// unbinds the index buffer
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
void IndexBuffer::SetupIndexBuffer(const unsigned int* indices, u32 drawType)
{
	glGenBuffers(1, &m_renderBufferId);// generates a buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderBufferId);// binds that buffer to the context
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(u32), indices, drawType);
	//passes in the data of the index array
}

void IndexBuffer::BufferData(const void* data, size_t size, u32 drawType)
{
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, drawType); // adds data to the bound buffer
}

/**
*Binds this class instance's index buffer.
*/
void IndexBuffer::Bind() const
{
	m_indexMtx.lock();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderBufferId);//binds the index buffer to the current context
	m_indexMtx.unlock();
}
/**
*Unbinds this class instance's index buffer.
*/
void IndexBuffer::Unbind() const
{
	m_indexMtx.lock();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//unbinds the index buffer from the current context
	m_indexMtx.unlock();
}
