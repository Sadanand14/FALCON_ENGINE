#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "BasicComponent.h"

#include <PipeLine/Mesh.h>

typedef boost::container::vector<glm::vec3> boundingVector;


/**
*Structure Definition for holding data needed to render the entity.
*/
struct RenderComponent : public BasicComponent
{
	Mesh* m_mesh;

	//bounding Box stuff
	boundingVector m_boundingCorners;

	inline boundingVector* GetBounds() { return &m_boundingCorners; }

	RenderComponent() : m_mesh(nullptr) { m_boundingCorners.reserve(8); }
	~RenderComponent() {}
	void CalculateBounds()
	{
		Vertex* vertexArr = m_mesh->m_vertexArray.data();

		float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
		float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;
		for (unsigned int i = 0; i < m_mesh->m_vertexArray.size(); i++)
		{
			glm::vec3 pos = vertexArr[i].Position;
			if (minX > pos.x) minX = pos.x;
			if (maxX < pos.x) maxX = pos.x;

			if (minY > pos.y) minY = pos.y;
			if (maxY < pos.y) maxY = pos.y;

			if (minZ > pos.z) minZ = pos.z;
			if (maxZ < pos.z) maxZ = pos.z;
		}

		m_boundingCorners.push_back(glm::vec3(minX, maxY, minZ));
		m_boundingCorners.push_back(glm::vec3(maxX, maxY, minZ));
		m_boundingCorners.push_back(glm::vec3(minX, minY, minZ));
		m_boundingCorners.push_back(glm::vec3(maxX, minY, minZ));

		m_boundingCorners.push_back(glm::vec3(minX, maxY, maxZ));
		m_boundingCorners.push_back(glm::vec3(maxX, maxY, maxZ));
		m_boundingCorners.push_back(glm::vec3(minX, minY, maxZ));
		m_boundingCorners.push_back(glm::vec3(maxX, minY, maxZ));
	}
};

#endif // !1
