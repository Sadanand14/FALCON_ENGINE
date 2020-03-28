#include "Line.h"

/**
 * Basic line constructor
 */
Line::Line() : Renderable(), m_VBO(nullptr)
{

}

/**
 * Sets up the vao
 */
void Line::Setup()
{
	Renderable::Setup();
	m_material = fmemory::fnew<Material>();
	m_material->m_shader = fmemory::fnew<Shader>("../Falcon/Rendering/Shader/Line.vert", "../Falcon/Rendering/Shader/Line.frag");

	m_VBO = fmemory::fnew<VertexBuffer>(nullptr, sizeof(glm::vec3), GL_DYNAMIC_DRAW);

	m_VBO->Bind();
	m_VAO->AddVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0, 0);
	m_VBO->Unbind();
}

/**
 * Binds the VertexArray object held by the mesh along with the relevant vertex and index buffer.
 */
void Line::Bind()
{
	Renderable::Bind();
	m_VBO->Bind();
	m_VBO->BufferData(m_points.data(), m_points.size() * sizeof(glm::vec3), GL_DYNAMIC_DRAW);
	m_VBO->Unbind();
}

/**
 * Line destructor
 */
Line::~Line()
{
	fmemory::fdelete<VertexBuffer>(m_VBO);
}