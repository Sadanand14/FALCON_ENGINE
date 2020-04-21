#include "Mesh.h"
#include "Log.h"

// On g++, string.f contains the implementation of memcpy_s
#ifdef FL_PLATFORM_UNIX
#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>
#endif

/**
* Basic Mesh Constructor
*/
Mesh::Mesh() : m_VBO1(nullptr), m_VBO2(nullptr), m_VBO3(nullptr), m_IBO(nullptr)
{

}

/**
*Sets a vertex Array Object with the vertex and indexbuffer values in this mesh class.
*/
void Mesh::Setup()
{
	Renderable::Setup();
	m_VBO1 = fmemory::fnew<VertexBuffer>(m_vertexArray.data(), m_vertexArray.size() * sizeof(Vertex), GL_STATIC_DRAW);
	m_VBO2 = fmemory::fnew<VertexBuffer>(nullptr, sizeof(glm::mat4), GL_DYNAMIC_DRAW);
	m_VBO3 = fmemory::fnew<VertexBuffer>(nullptr, sizeof(glm::mat3), GL_DYNAMIC_DRAW);
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

	m_VBO3->Bind();
	for (u32 i = 0; i < 3; i++)
	{
		m_VAO->AddVertexAttribPointer(9 + i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), sizeof(glm::vec3) * i, 1);
	}
	m_VBO3->Unbind();
}

/**
 * Preallocates the world matrix vector with a specified size
 * @param maxMatrices - The maximum number of items in the world
 */
void Mesh::PreallocMatrixAmount(u32 maxMatrices)
{
	m_worldMats.resize(maxMatrices);
	m_normalMats.resize(maxMatrices);
}

/**
 * Adds a world and normal matrix to the list of matrices for instanced rendering
 * @param mat - The world matrix to add
 * @param normal - The normal matrix to add
 */
void Mesh::AddWorldAndNormalMatrix(const glm::mat4& mat, const glm::mat3& normal)
{
	m_worldMats.push_back(mat);
	m_normalMats.push_back(normal);
}

/**
 * Clears the meshes world matrices
 */
void Mesh::ClearWorldMatrices()
{
	m_worldMats.clear();
	m_normalMats.clear();
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
* Binds the VertexArray object held by the mesh along with the relevant vertex and index buffer.
*/
void Mesh::Bind()
{
	Renderable::Bind();
	m_VBO2->Bind();
	m_VBO2->BufferData(m_worldMats.data(), m_worldMats.size() * sizeof(glm::mat4), GL_DYNAMIC_DRAW);
	m_VBO2->Unbind();

	m_VBO3->Bind();
	m_VBO3->BufferData(m_normalMats.data(), m_normalMats.size() * sizeof(glm::mat3), GL_DYNAMIC_DRAW);
	m_VBO3->Unbind();
}


/**
* Returns the vertexArray into the vector passed.
* @param glm::vec3 vector bufffer to copy data into.
*/
void Mesh::GetVertexPositionsArray(std::vector < glm::vec3, fmemory::STLAllocator<glm::vec3>>& vertPosArray) const
{

	vertPosArray.resize(m_vertexArray.size());
	for (u32 itr = 0; itr < m_vertexArray.size(); ++itr)
	{
#ifdef FL_PLATFORM_WINDOWS
		memcpy_s(&vertPosArray[itr], sizeof(glm::vec3), &m_vertexArray[itr], sizeof(glm::vec3));
#else
		memcpy(&vertPosArray[itr], &m_vertexArray[itr], sizeof(glm::vec3));
#endif
	}

	//return &vertPosArray[0];
}

/**
* Mesh class Destructor.
*/
Mesh::~Mesh()
{
	fmemory::fdelete<VertexBuffer>(m_VBO2);
	fmemory::fdelete<VertexBuffer>(m_VBO1);
	fmemory::fdelete<IndexBuffer>(m_IBO);
}
