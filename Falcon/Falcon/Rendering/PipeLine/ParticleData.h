#ifndef PARTICLEDATA_H
#define PARTICLEDATA_H

#include <glm/glm.hpp>

struct ParticleData
{
	glm::vec3 m_pos = glm::vec3(0.0f);
	float m_size = 1.0f;
	float m_opacity = 1.0f;
	float m_age = 0.0f;
};

#endif