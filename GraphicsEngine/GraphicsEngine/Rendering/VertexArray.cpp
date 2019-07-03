#include "VertexArray.h"


VertexArray::VertexArray():m_rendererID(0), m_vertexbuffer(nullptr)
{
	glGenVertexArrays(1, &m_rendererID);// generates a vertex Array
	glBindVertexArray(0);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_rendererID);// deletes the vertex array
}

//adds a buffer and its respective layout to the vertex array object.
//note that this function autatically has to bind the vertex buffer and itself to the current context
void VertexArray::AddBuffer( VertexBuffer* const vb, const VertexLayout& vl)
{
	glBindVertexArray(m_rendererID);		// binds the vertex array
	vb->Bind();								// binds the passed in vertex buffer
	const auto& elements = vl.GetElements();//gets the attributes structure list from the vertex layout
	unsigned int offset = 0;				// sets the initial offset to zero
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];	// takes in attributes one by one
		glEnableVertexAttribArray(i);		// enables the locations for those attributes.

		glVertexAttribPointer(i, element.count, element.type, element.normalized, vl.GetStride(), (const void*)offset);
		//designates the various properties of each attribute, check docs.gl for more info

		offset += element.count * VertexElements::GetSizeofType(element.type);// increments offset for each attribute added
	}
	
	m_vertexbuffer = vb;			//sets the vertex buffer pointer to the one thats been added
	m_vertexbuffer->Unbind();       // unbinds the vertex buffer
	glBindVertexArray(0);			// unbinds that vertex array
}

//binds both the buffer and the vertexbuffer inside if its available
void VertexArray::Bind() const
{
	glBindVertexArray(m_rendererID);// binds the vertex array object to the current context
	if (m_vertexbuffer) m_vertexbuffer->Bind();// if it has a vertex buffer, binds it
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);// unbinds the array
	if(m_vertexbuffer)m_vertexbuffer->Unbind();// if it has a vertex buffer, unbinds it
}
