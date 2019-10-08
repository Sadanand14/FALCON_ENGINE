#include "VertexArray.h"


/**
* Main Constructor for the Vertex Array Object Class. 
*/
VertexArray::VertexArray():m_rendererID(0), m_vertexbuffer(nullptr)
{
	glGenVertexArrays(1, &m_rendererID);// generates a vertex Array
	glBindVertexArray(0);
}

/**
* Main Destructor for the Vertex Array Object Class.
*/
VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_rendererID);// deletes the vertex array
}

/**
* This function attaches the provided vertex buffer to this vertex array object.
* 
* @param[in] A constant pointer to an object of type Vertex Buffer.
*/
void VertexArray::AddBuffer( VertexBuffer* const vb)
{
	glBindVertexArray(m_rendererID);		// binds the vertex array
	m_vertexbuffer = vb;			//sets the vertex buffer pointer to the one thats been added
	m_vertexbuffer->Bind();								// binds the passed in vertex buffer
	SetupVertextAttribPointers();
	
	
	m_vertexbuffer->Unbind();       // unbinds the vertex buffer
	glBindVertexArray(0);			// unbinds that vertex array
}


/**
* This function Sets up the Attribute layout for the vertex buffer provided.
*/
void VertexArray::SetupVertextAttribPointers()
{

	//Vertex attribute pointers	
	//Positions
	glEnableVertexAttribArray(0);	// enables the locations for those attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);	//designates the various properties of each attribute, check docs.gl for more info
	//Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// Texture Coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	//Tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	//Bitangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
}

/**
* Binds this Vertex array Object.
*/
void VertexArray::Bind() const
{
	glBindVertexArray(m_rendererID);// binds the vertex array object to the current context
	if (m_vertexbuffer) m_vertexbuffer->Bind();// if it has a vertex buffer, binds it
}

/**
* Unbinds this Vertex Array Object.
*/
void VertexArray::Unbind() const
{
	glBindVertexArray(0);// unbinds the array
	if(m_vertexbuffer) m_vertexbuffer->Unbind();// if it has a vertex buffer, unbinds it
}
