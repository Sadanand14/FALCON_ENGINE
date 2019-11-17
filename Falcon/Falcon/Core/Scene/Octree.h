#ifndef OCTREE_H
#define OCTREE_H

#include "glm/glm.hpp"
#include <boost/container/vector.hpp>
#include <EntityInterface.h>
#include <Memory/fmemory.h>
//#include <boost/iterator.hpp>
#include <Scene/SceneGraph.h>

namespace Scene
{
	class OctreeNode;
	class Octree;

	typedef boost::container::vector<OctreeNode*, fmemory::StackSTLAllocator<OctreeNode*>> OctreeNodeVector;

	class OctreeNode
	{
		friend class Octree;
		friend bool CheckEntityPosInNode(OctreeNode* node, Entity* entity);

		entityVector m_entities;
		glm::vec3 m_nearTopLeft;
		glm::vec3 m_farBottomRight;
		OctreeNode* m_parent;
		OctreeNodeVector m_childNodes;

		inline void SetEntities(entityVector entityList) { m_entities = entityList; }
		void Subdivide(float minSide);
		void Segregate();
		void AddEntity(Entity* entity);
		void RemoveEntity(Entity* entity);

		inline void SetParent(OctreeNode* parent) { m_parent = parent; }
		inline const OctreeNode* GetParent() const { return m_parent; }

	public:
		OctreeNode(glm::vec3 nearTopLeft, glm::vec3 farBottomRight);
		~OctreeNode();
	};


	class Octree
	{
		SceneGraph* m_scene;
		glm::vec3 m_nearTopLeft;
		glm::vec3 m_farBottomRight;
		OctreeNode* m_rootNode;
		entityVector::iterator FindEntityInVector(Entity* entity, entityVector vector);
		void UpdateEntityPosition(OctreeNode* node, Entity* entity);
		OctreeNode* FindNode(Entity* entity);
		void FilterEntities(entityVector& entities);
		void AddEntity(Entity* entity);
		void RemoveEntity(Entity* entity);
	public:

		Octree(glm::vec3 nearTopLeft, glm::vec3 farBottomRight, float minSide, SceneGraph* scene);
		void Distribute();
		void Update();
		~Octree();
	};

}

#endif // !1