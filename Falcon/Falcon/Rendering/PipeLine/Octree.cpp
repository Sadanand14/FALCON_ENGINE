#include "Octree.h"

OctreeNode::OctreeNode(glm::vec3 nearTopLeft, glm::vec3 farBottomRight) :m_nearTopLeft(nearTopLeft), m_farBottomRight(farBottomRight), m_parent(nullptr)
{

}

OctreeNode::~OctreeNode() 
{
	
}

Octree::Octree(glm::vec3 nearTopLeft,glm::vec3 farBottomRight, float  minSide) : m_nearTopLeft(nearTopLeft), m_farBottomRight(farBottomRight), m_minSide(minSide)
{
	m_rootNode = new OctreeNode(m_nearTopLeft, m_farBottomRight);
	
}

Octree::~Octree()
{
	delete m_rootNode;
}

void Octree::Subdivide() 
{
	
}