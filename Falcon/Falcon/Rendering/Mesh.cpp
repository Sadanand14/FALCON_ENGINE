#include "Mesh.h"
#include "Log.h"

Mesh::Mesh():m_VAO(nullptr), m_VBO1(nullptr), m_VBO2(nullptr), m_IBO(nullptr)
{

}

void Mesh::SetupMesh()
{
	m_VAO = new VertexArray();
	m_VAO->Bind();
	m_VBO1 = new VertexBuffer(m_vertexArray.data(), m_vertexArray.size() * sizeof(Vertex), GL_STATIC_DRAW);
	m_VBO2 = new VertexBuffer(nullptr, sizeof(glm::mat4), GL_DYNAMIC_DRAW);
	m_IBO = new IndexBuffer(m_indexArray.data(), m_indexArray.size());

	m_VBO1->Bind();
	m_VAO->AddVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0, 0);
	m_VAO->AddVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, TexCoords), 0);
	m_VAO->AddVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal), 0);
	m_VAO->AddVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Tangent), 0);
	m_VAO->AddVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Bitangent), 0);
	m_VBO1->Unbind();

	m_VBO2->Bind();
	for(u32 i = 0; i < 4; i++)
	{
		m_VAO->AddVertexAttribPointer(5 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), sizeof(glm::vec4) * i, 1);
	}
	m_VBO2->Unbind();
}

void Mesh::PreallocMatrixAmount(u32 maxMatrices)
{
	m_worldMats.resize(maxMatrices);
}

void Mesh::AddWorldMatrix(const glm::mat4 &mat)
{
	m_worldMats.push_back(mat);
}

void Mesh::ClearWorldMatrices()
{
	m_worldMats.clear();
}

u32 Mesh::GetWorldMatrixAmount()
{
	return m_worldMats.size();
}

void Mesh::Bind()
{
	// Draw Mesh
	m_VAO->Bind();

	m_VBO2->Bind();
	m_VBO2->BufferData(m_worldMats.data(), m_worldMats.size() * sizeof(glm::mat4), GL_DYNAMIC_DRAW);
	m_VBO2->Unbind();
}

Mesh::~Mesh()
{
	delete m_VAO;
	delete m_VBO2;
	delete m_VBO1;
	delete m_IBO;
}
