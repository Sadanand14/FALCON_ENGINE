#ifndef OCTREE_H
#define OCTREE_H

#include "glm/glm.hpp"
#include <boost/container/vector.hpp>
#include <EntityInterface.h>
#include <Memory/fmemory.h>

struct OctreeNode;

typedef boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>> entityVector;
typedef boost::container::vector<OctreeNode*, fmemory::StackSTLAllocator<OctreeNode*>> OctreeNodeVector;

struct OctreeNode 
{
	entityVector containedEntities;
	glm::vec3 m_nearTopLeft;
	glm::vec3 m_farBottomRight;
	OctreeNode* m_parent;
	OctreeNodeVector m_childNodes;

	OctreeNode(glm::vec3 nearTopLeft, glm::vec3 farBottomRight);
	~OctreeNode();
};


class Octree 
{
	glm::vec3 m_nearTopLeft;
	glm::vec3 m_farBottomRight;
	float m_minSide;
	OctreeNode* m_rootNode;

public:
	void Subdivide();
	Octree(glm::vec3 nearTopLeft, glm::vec3 farBottomRight, float minSide);
	~Octree();
};


#endif // !1

