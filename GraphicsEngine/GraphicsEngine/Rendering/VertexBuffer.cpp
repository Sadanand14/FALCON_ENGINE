#include "VertexBuffer.h"

//generate and store data into a vertexbuffer
VertexBuffer::VertexBuffer(const void* databuffer, unsigned int size)
{
	glGenBuffers(1, &renderBufferId);			   //generates a vertex buffer 
	glBindBuffer(GL_ARRAY_BUFFER, renderBufferId); //binds the vertex buffer to the current context
	glBufferData(GL_ARRAY_BUFFER, size, databuffer, GL_STATIC_DRAW); // adds data to the bound buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);				// unbinds the buffer from the current context.
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &renderBufferId);		// deletes the buffer
}

//bind the vertex buffer
void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, renderBufferId);// binds the buffer to the current context
}


void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);			//unbinds the buffer frpom the current context
}

