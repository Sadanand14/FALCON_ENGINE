#include "VertexArray.h"


/**
* Main Constructor for the Vertex Array Object Class.
*/
VertexArray::VertexArray():m_rendererID(0)
{
	glGenVertexArrays(1, &m_rendererID);// generates a vertex Array
}

/**
* Main Destructor for the Vertex Array Object Class.
*/
VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_rendererID);// deletes the vertex array
}

void VertexArray::AddVertexAttribPointer(u32 loc, u32 size, u32 type, u32 normalize, u32 byteSize, u32 offset, u32 divisor)
{
	//Vertex attribute pointers
	switch(type)
	{
		case GL_UNSIGNED_INT:
		case GL_INT:
			glVertexAttribIPointer(loc, size, type, byteSize, (const void*)offset);	//designates the various
			break;
		case GL_DOUBLE:
			glVertexAttribLPointer(loc, size, type, byteSize, (const void*)offset);	//designates the various
			break;
		case GL_FLOAT:
		default:
			glVertexAttribPointer(loc, size, type, normalize, byteSize, (const void*)offset);	//designates the various
			break;
	}
	glEnableVertexAttribArray(loc);	// enables the locations for those attributes
	glVertexAttribDivisor(loc, divisor); //Sets the vertex attrib divsor
}

/**
* Binds this Vertex array Object.
*/
void VertexArray::Bind() const
{
	glBindVertexArray(m_rendererID);// binds the vertex array object to the current context
}

/**
* Unbinds this Vertex Array Object.
*/
void VertexArray::Unbind() const
{
	glBindVertexArray(0);// unbinds the array
}
