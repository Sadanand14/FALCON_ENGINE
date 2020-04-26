#include "Terrain.h"
#include "Components/TransformComponent.h"

Terrain::Terrain()
	:m_physicsC(nullptr)
{}

Terrain::~Terrain()
{
	if(m_physicsC !=nullptr)
		fmemory::fdelete<PhysicsComponent>(m_physicsC);
}

void Terrain::AddPhysicsToTerrain(const glm::vec3* vertexData, const int& vertcount, const int& vertexStride, const u32* indexData, const int& indexCount, const int& indexStride)
{
	try
	{
		m_physicsC = fmemory::fnew<PhysicsComponent>();

		m_physicsC->SetMeshColliderWithTriangleMeshes(vertexData, vertcount, vertexStride,
			indexData, indexCount, indexStride, glm::vec3(1, 1, 1));
		m_physicsC->MakeDrivableSurface();

		Transform t(glm::vec3(0, 0, 0), glm::quat(1,0,0,0), glm::vec3(1, 1, 1));
		m_physicsC->SetPhysicsBodyType(&t, physics::PhysicsBodyType::ESTATIC_BODY);
	}
	catch(std::exception& e)
	{
		FL_ENGINE_ERROR("ERROR: Failed to create physics for the terrain in {0} {1}. \n {2}",
						__FUNCTION__, __LINE__, e.what());
	}
}
