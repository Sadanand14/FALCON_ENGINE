#include "Particle.h"
#include <Core/Components/ParticleEmitterComponent.h>

/**
 * Basic Particle Constructor
 */
Particle::Particle() :m_VAO(nullptr), m_VBO(nullptr)
{

}

/**
 * Sets a vertex Array Object with the vertex and indexbuffer values in this mesh class.
 */
void Particle::Setup()
{
	m_VAO = fmemory::fnew<VertexArray>();
	m_VAO->Bind();
	m_VBO = fmemory::fnew<VertexBuffer>(nullptr, sizeof(glm::vec3), GL_DYNAMIC_DRAW);

	m_VBO->Bind();
	m_VAO->AddVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleData), offsetof(ParticleData, m_pos), 1);
	m_VAO->AddVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleData), offsetof(ParticleData, m_size), 1);
	m_VAO->AddVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleData), offsetof(ParticleData, m_opacity), 1);
	m_VBO->Unbind();
}

/**
 * Preallocates the particle data vector with a specified size
 * @param maxParticleData - The maximum number of items in the world
 */
void Particle::PreallocParticleDataAmount(u32 maxParticleData)
{
	m_particles.resize(maxParticleData);
}

/**
 * Adds particle data to the list of data for instanced rendering
 * @param data - The particle data toadd
 */
void Particle::AddParticleData(const ParticleData &data)
{
	m_particles.push_back(data);
}

/**
 * Clears the particle data vector
 */
void Particle::ClearParticleData()
{
	m_particles.clear();
}

/**
 * Gets the size of the particle data array
 * @return The size of the particle data array
 */
u32 Particle::GetParticleDataAmount()
{
	return m_particles.size();
}

/**
 * Sets the material of the mesh
 * @param mat - The new material for the mesh
 */
void Particle::SetMaterial(Material* mat)
{
	m_material = mat;
}

/**
 * Gets the material of the mesh
 * @return - The material of the mesh
 */
Material* Particle::GetMaterial()
{
	return m_material;
}

void Particle::SetWorldMatrix(glm::mat4 mat)
{
	m_worldMat = mat;
}
glm::mat4 & Particle::GetWorldMatrix()
{
	return m_worldMat;
}

/**
 * Binds the VertexArray object held by the mesh along with the relevant vertex and index buffer.
 */
void Particle::Bind()
{
	// Draw Particle
	m_VAO->Bind();

	m_VBO->Bind();
	m_VBO->BufferData(m_particles.data(), m_particles.size() * sizeof(ParticleData), GL_DYNAMIC_DRAW);
	m_VBO->Unbind();

	if (m_material != nullptr)
		m_material->Bind();
}

/**
 * Particle class Destructor.
 */
Particle::~Particle()
{
	fmemory::fdelete<VertexArray>(m_VAO);
	fmemory::fdelete<VertexBuffer>(m_VBO);
}