#include "Mesh.h"
#include "Log.h"

Mesh::Mesh():m_VAO(nullptr), m_VBO(nullptr), m_IBO(nullptr)
{

}

void Mesh::SetupMesh()
{
	m_VAO = new VertexArray();
	m_VAO->Bind();
	m_VBO = new VertexBuffer(m_vertexArray.data(), m_vertexArray.size() * sizeof(Vertex));
	m_IBO = new IndexBuffer(m_indexArray.data(), m_indexArray.size());
	m_VAO->AddBuffer(m_VBO);
	m_VAO->Unbind();
}



void Mesh::DrawMesh(Shader shader)
{
	// Draw Mesh
	m_VAO->Bind();
	m_IBO->Bind();
	//glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);


	// Set back to default
	//glActiveTexture(GL_TEXTURE0);
}

Mesh::~Mesh()
{
	delete m_VAO;
	delete m_VBO;
	delete m_IBO;
}
