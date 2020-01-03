#ifndef PARTICLE_H
#define PARTICLE_H

#include <framework.h>
#include <vector>
#include <string>
#include <boost/container/vector.hpp>

#include <Memory/fmemory.h>
#include "Shader.h"
#include "Material.h"
#include "Rendering/PipeLine/ParticleData.h"

#include <BufferDefinitions/VertexArray.h>
#include <BufferDefinitions/VertexBuffer.h>
#include <BufferDefinitions/IndexBuffer.h>
#include <BufferDefinitions/VertexLayout.h>

#include <Types.h>

/**
 * Particle Class tp store Particle Data for Renderables.
 */
class Particle {

private:

	//Experimental

	//Render Data
	VertexArray* m_VAO = nullptr;
	VertexBuffer* m_VBO = nullptr;
	Material* m_material = nullptr;
	boost::container::vector<ParticleData> m_particles;
	glm::mat4 m_worldMat;
	//std::string m_path;
	//std::string m_jsonPath;

	//Functions

public:
	//Mesh Data
	boost::container::vector<Vertex> m_vertexArray;

	Particle();
	~Particle();

	//Functions
	void Setup();
	void PreallocParticleDataAmount(u32 maxParticleData);
	void AddParticleData(const ParticleData &data);
	void ClearParticleData();
	u32 GetParticleDataAmount();
	void SetMaterial(Material* mat);
	Material* GetMaterial();
	void SetWorldMatrix(glm::mat4 mat);
	glm::mat4 & GetWorldMatrix();
	void Bind();

	//inline const std::string& GetJsonPath() const { return m_jsonPath; }
	//inline void SetJsonPath(const std::string& jsonPath) { m_jsonPath = jsonPath; }
	//inline const std::string& GetPath() const { return m_path; }
	//inline void SetPath(const std::string& path) { m_path = path; }

};

#endif //!PARTICLE_H