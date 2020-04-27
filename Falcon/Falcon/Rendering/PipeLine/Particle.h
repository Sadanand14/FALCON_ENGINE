#ifndef PARTICLE_H
#define PARTICLE_H

#include <System/Types.h>
#include <framework.h>
#include <vector>
#include <string>
#include "System/Types.h"

#include <Memory/fmemory.h>
#include "Rendering/PipeLine/ParticleData.h"
#include <RenderPasses/Renderable.h>

/**
 * Particle Class tp store Particle Data for Renderables.
 */
class Particle : public Renderable
{
private:
	//Render Data
	VertexBuffer* m_VBO = nullptr;
	boost::container::vector<ParticleData> m_particles;
	glm::mat4 m_worldMat;

public:
	Particle();
	~Particle();

	//Functions
	void Setup() override;
	void PreallocParticleDataAmount(u32 maxParticleData);
	void AddParticleData(const ParticleData &data);
	void ClearParticleData();
	u32 GetParticleDataAmount();
	void SetWorldMatrix(glm::mat4 mat);
	glm::mat4 & GetWorldMatrix();
	void Bind() override;

	//inline const std::string& GetJsonPath() const { return m_jsonPath; }
	//inline void SetJsonPath(const std::string& jsonPath) { m_jsonPath = jsonPath; }
	//inline const std::string& GetPath() const { return m_path; }
	//inline void SetPath(const std::string& path) { m_path = path; }

};

#endif //!PARTICLE_H