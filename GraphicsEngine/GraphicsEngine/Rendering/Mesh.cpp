#include "Mesh.h"
#include "Log.h"


/**
* Constructor for Mesh Class.
*/
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures) 
	: m_vertices(vertices) , m_indices(indices) , m_textures(textures),
	  m_VAO(nullptr), m_VBO(nullptr), m_IBO(nullptr)
{
	m_VAO = new VertexArray();
	m_VAO->Bind();
	m_VBO = new VertexBuffer(&m_vertices[0], m_vertices.size() * sizeof(Vertex));
	m_IBO = new IndexBuffer(m_indices, m_indices.size());
	m_VAO->AddBuffer(m_VBO);
	m_VAO->Unbind();
	FL_ENGINE_INFO("INFO: Mesh set up correctly.");
}

/**
* Destructor for Mesh Class.
*/
Mesh::~Mesh()
{
	delete m_VAO;
	delete m_VBO;
	delete m_IBO;
}

//TODO: Have to modify the function to remove the draw call from it.
void Mesh::DrawMesh(Shader shader)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = m_textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); 
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); 
		else if (name == "texture_height")
			number = std::to_string(heightNr++); 

	    // Set Sampler
		glUniform1i(glGetUniformLocation(shader.m_programID, (name + number).c_str()), i);
		// Bind Texture
		glBindTexture(GL_TEXTURE_2D, m_textures[i].textureID);
	}

	// Draw Mesh
	m_VAO->Bind();
	m_IBO->Bind();
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
	m_IBO->Unbind();
	m_VAO->Unbind();

	// Set back to default
	glActiveTexture(GL_TEXTURE0);
}
