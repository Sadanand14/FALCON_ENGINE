#include "Renderable.h"

Renderable::Renderable() : m_VAO(nullptr)
{

}

/**
 * Sets up the VAO for the mesh
 */
void Renderable::Setup()
{
	m_VAO = fmemory::fnew<VertexArray>();
	m_VAO->Bind();
}

/**
 * Binds the VertexArray object and the material
 */
void Renderable::Bind()
{
	m_VAO->Bind();

	if (m_material != nullptr)
		m_material->Bind();
}

Renderable::~Renderable()
{
	delete[] m_vertexArray;
	fmemory::fdelete<VertexArray>(m_VAO);
}