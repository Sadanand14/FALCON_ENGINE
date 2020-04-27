#ifndef PARTICLE_EMITTER_COMPONENT_H
#define PARTICLE_EMITTER_COMPONENT_H

#include <boost/circular_buffer.hpp>
#include "BasicComponent.h"
#include <PipeLine/Particle.h>
#include "Rendering/PipeLine/ParticleData.h"

/**
 * Structure definition for particle components
 */
struct ParticleEmitterComponent: public BasicComponent
{
	boost::circular_buffer<ParticleData> m_particleBuffer;
	u32 m_emissionRate = 5; /// The rate of emmision of new particles

	glm::vec3 m_velocity = glm::vec3(0.0f, 1.0f, 0.0f); /// The velocity for the particles
	float m_timer = 0.0f; /// The timer for spawning new particles
	float m_particlePlayTimer = 60.0f; /// The timer for how long the particle effect should play for (if not looping)
	float m_particlePlayProgress = 0.0f; /// The progress of the particles playing time
	float m_particleLifetime = 5.0f; /// The lifetime of one particle
	float m_startSize = 1.0f; /// The starting size of a particle
	float m_endSize = 3.0f; /// The ending size of a particle
	float m_fadeTo = 0.0f; /// The value to fade the particle alpha to as it progresses over time
	glm::vec3 m_spawnRange = glm::vec3(0.0f, 0.0f, 0.0f); /// The maximum spawn distance for the particle

	bool m_loop = false; /// If the particle effect should loop
	bool m_active = true; /// If the particle effect is active
	Particle* m_particle = nullptr;

	ParticleEmitterComponent() : m_particleBuffer(20.0f) {}
	~ParticleEmitterComponent() {
		if (m_particle != nullptr)//fmemory::fdelete<Particle>(m_particle); }
			delete m_particle;
	}
	void SetMaxParticles(u32 maxParticles) { m_particleBuffer.set_capacity(maxParticles); }
};

#endif