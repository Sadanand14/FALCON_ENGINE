#ifndef TERRAIN_H
#define TERRAIN_H

#include "Types.h"
#include "Components/PhysicsComponent.h"

class Terrain
{
private:
	PhysicsComponent* m_physicsC = nullptr;
public:
	Terrain();
	~Terrain();
	void AddPhysicsToTerrain(const glm::vec3* vertexData, const int& vertcount, const int& vertexStride,
									const u32* indexData, const int& indexCount, const int& indexStride);
};

#endif TERRAIN_H