#include "Mesh.h"
#include "Log.h"


// On g++, string.f contains the implementation of memcpy_s
#ifdef FL_PLATFORM_UNIX
#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>
#endif

/**
*Basic Mesh Constructor
*/
Mesh::Mesh() :m_VAO(nullptr), m_VBO1(nullptr), m_VBO2(nullptr), m_IBO(nullptr)
{
	m_vertexArray.reserve(110000);
	m_indexArray.reserve(330000);
	m_indexOffsets.reserve(10000);
}

/**
*Sets a vertex Array Object with the vertex and indexbuffer values in this mesh class.
*/
void Mesh::SetupMesh()
{
	m_VAO = fmemory::fnew<VertexArray>();
	m_VAO->Bind();
	m_VBO1 = fmemory::fnew<VertexBuffer>(m_vertexArray.data(), m_vertexArray.size() * sizeof(Vertex), GL_STATIC_DRAW);
	m_VBO2 = fmemory::fnew<VertexBuffer>(nullptr, sizeof(glm::mat4), GL_DYNAMIC_DRAW);
	m_IBO = fmemory::fnew<IndexBuffer>(m_indexArray.data(), m_indexArray.size());

	m_VBO1->Bind();
	m_VAO->AddVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0, 0);
	m_VAO->AddVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, TexCoords), 0);
	m_VAO->AddVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal), 0);
	m_VAO->AddVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Tangent), 0);
	m_VAO->AddVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Bitangent), 0);
	m_VBO1->Unbind();

	m_VBO2->Bind();
	for (u32 i = 0; i < 4; i++)
	{
		m_VAO->AddVertexAttribPointer(5 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), sizeof(glm::vec4) * i, 1);
	}
	m_VBO2->Unbind();
}

/**
 * Preallocates the world matrix vector with a specified size
 * @param maxMatrices - The maximum number of items in the world
 */
void Mesh::PreallocMatrixAmount(u32 maxMatrices)
{
	m_worldMats.resize(maxMatrices);
}

/**
 * Adds a world matrix to the list of matrices for instanced rendering
 * @param mat - The world matrix to add
 */
void Mesh::AddWorldMatrix(const glm::mat4& mat)
{
	m_worldMats.push_back(mat);
}

/**
 * Clears the meshes world matrices
 */
void Mesh::ClearWorldMatrices()
{
	m_worldMats.clear();
}

/**
 * Gets the size of the world matrices array
 * @return The size of the world matrix array
 */
u32 Mesh::GetWorldMatrixAmount()
{
	return m_worldMats.size();
}

/**
 * Sets the material of the mesh
 * @param mat - The new material for the mesh
 */
void Mesh::SetMaterial(Material* mat)
{
	m_material = mat;
}

/**
 * Gets the material of the mesh
 * @return - The material of the mesh
 */
Material* Mesh::GetMaterial()
{
	return m_material;
}

/**
* Binds the VertexArray object held by the mesh along with the relevant vertex and index buffer.
*/
void Mesh::Bind()
{
	// Draw Mesh
	m_VAO->Bind();

	m_VBO2->Bind();
	m_VBO2->BufferData(m_worldMats.data(), m_worldMats.size() * sizeof(glm::mat4), GL_DYNAMIC_DRAW);
	m_VBO2->Unbind();

	if (m_material != nullptr)
		m_material->Bind();
}

glm::vec3* Mesh::GetVertexPositionsArray()
{
	std::vector < glm::vec3, fmemory::STLAllocator<glm::vec3>> vertPosArray;
	vertPosArray.resize(m_vertexArray.size());

	for (u32 itr = 0; itr < m_vertexArray.size(); ++itr)
	{
#ifdef FL_PLATFORM_WINDOWS
		memcpy_s(&vertPosArray[itr], sizeof(glm::vec3), &m_vertexArray[itr], sizeof(glm::vec3));
#else
		memcpy(&vertPosArray[itr],  &m_vertexArray[itr], sizeof(glm::vec3));
#endif
	}

	return &vertPosArray[0];
}

/**
* Mesh class Destructor.
*/
Mesh::~Mesh()
{
	fmemory::fdelete<VertexArray>(m_VAO);
	fmemory::fdelete<VertexBuffer>(m_VBO2);
	fmemory::fdelete<VertexBuffer>(m_VBO1);
	fmemory::fdelete<IndexBuffer>(m_IBO);
}
