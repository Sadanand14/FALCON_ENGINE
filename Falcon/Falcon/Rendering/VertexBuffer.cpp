#include "VertexBuffer.h"


/**
* Main Constructor for the Vertex Buffer
* Generates a buffer to store the provided vertex data 
*
*param[in] A Void pointer pointing to the data.
*param[in] A u32 type integer indicating size of data. 
*param[in] A u32 type integer indicating drawtype.
*/
VertexBuffer::VertexBuffer(const void* databuffer, size_t size, u32 drawType)
	:m_renderBufferId(0)
{
	glGenBuffers(1, &m_renderBufferId);			   //generates a vertex buffer
	BufferData(databuffer, size, drawType);
}

/**
* Destructor for the Vertex Buffer Class
*/
VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_renderBufferId);		// deletes the buffer
}

/**
* This function the Vertex BUffer held by its instance to the Array buffer
*/
void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_renderBufferId);// binds the buffer to the current context
}

/**
* This function unbinds the array buffer.
*/
void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);			//unbinds the buffer frpom the current context
}

void VertexBuffer::BufferData(const void* data, size_t size, u32 drawType)
{
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, drawType); // adds data to the bound buffer
	Unbind();
}