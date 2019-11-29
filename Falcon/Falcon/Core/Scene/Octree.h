#ifndef OCTREE_H
#define OCTREE_H

#include "glm/glm.hpp"
#include <boost/container/vector.hpp>
#include <EntityInterface.h>
#include <Memory/fmemory.h>
//#include <boost/iterator.hpp>
#include <Scene/SceneGraph.h>
#include <Camera.h>

namespace Scene
{
	class OctreeNode;
	class Octree;

	typedef boost::container::vector<OctreeNode*, fmemory::StackSTLAllocator<OctreeNode*>> OctreeNodeVector;

	class OctreeNode
	{
		friend class Octree;
		friend bool CheckEntityPosInNode(OctreeNode* node, Entity* entity);
		friend bool CheckBounds(OctreeNode* node, glm::vec3 NTL , glm::vec3 FBR);

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
		entityVector m_entities;
		float m_minSide;
		glm::vec3 m_nearTopLeft;
		glm::vec3 m_farBottomRight;
		OctreeNode* m_rootNode;
		Camera* m_camera;
		glm::mat4 m_projection;

		//plane Array storing plane equation's coefficients in the order x,y,z,w	
		glm::vec4* m_planeArr;

		//entityVector::iterator FindEntityInVector(Entity* entity, entityVector vector);
		void GetPlanes();
		void UpdateEntityPosition(OctreeNode* node, Entity* entity);
		OctreeNode* FindNode(Entity* entity)const;
		void FilterEntities(entityVector& entities);
		void AddEntity(Entity* entity);
		void RemoveEntity(Entity* entity);
		void AssignNode(Entity* entity);
	public:

		inline void SetProjection(glm::mat4 proj) { m_projection = proj; }
		Octree(glm::vec3 nearTopLeft, glm::vec3 farBottomRight, float minSide, SceneGraph* scene, Camera* camera);
		void Distribute();
		void Update();
		~Octree();
	};

	//entityVector::iterator FindEntityInVector(Entity* entity, entityVector vector);
}

#endif // !1