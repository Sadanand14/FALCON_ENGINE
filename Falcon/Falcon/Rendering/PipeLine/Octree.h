#ifndef OCTREE_H
#define OCTREE_H

#include <System/Types.h>
#include <EntityInterface.h>
#include <Memory/fmemory.h>
//#include <boost/iterator.hpp>
#include <Scene/SceneGraph.h>
#include <Camera.h>

namespace Rendering
{
	//forward declarations
	class OctreeNode;
	class Octree;

	typedef boost::container::vector<OctreeNode*, fmemory::StackSTLAllocator<OctreeNode*>> OctreeNodeVector;

	/**
	* Data structure to hold all data and implementations related to a single node inside our dynamic Octree
	*/
	class OctreeNode
	{
		friend class Octree;
		
		friend bool CheckEntityPosInNode(OctreeNode* node, Entity* entity);
		friend bool CheckBounds(OctreeNode* node, glm::vec3 NTL, glm::vec3 FBR);
		static unsigned int nodeCount;

		static unsigned int m_nodeCount;
		Scene::entityVector m_entities;
		glm::vec3 m_centre;
		float m_radius;
		glm::vec3 m_nearTopLeft;
		glm::vec3 m_farBottomRight;
		OctreeNode* m_parent;
		OctreeNodeVector m_childNodes;

		void Subdivide(float minSide);


		inline void SetEntities(Scene::entityVector entityList) { m_entities = entityList; }
		inline void SetParent(OctreeNode* parent) { m_parent = parent; }
		inline const OctreeNode* GetParent() const { return m_parent; }

	public:
		static inline unsigned int GetCount() {return m_nodeCount; }
		OctreeNode(glm::vec3 nearTopLeft, glm::vec3 farBottomRight);
		~OctreeNode();
	};

	typedef boost::container::vector<glm::vec4, fmemory::StackSTLAllocator<glm::vec4>> planeArray;

	/**
	* Data structure to hold all data and implementations related to the entire octree.
	*/
	class Octree
	{
		Scene::SceneGraph* m_scene;
		Scene::entityVector m_renderables, m_viewables;
		float m_minSide;
		glm::vec3 m_nearTopLeft;
		glm::vec3 m_farBottomRight;
		OctreeNode* m_rootNode;
		Camera* m_camera;
		glm::mat4 m_projection;
		boost::circular_buffer<OctreeNode> m_freeNodes;
		boundingVector defaultVolume;

		//plane Array storing plane equation's coefficients in the order x,y,z,w
		planeArray m_planeArr;

		//entityVector::iterator FindEntityInVector(Entity* entity, entityVector vector);
		OctreeNode* FindNode(Entity* entity)const;
		void GetPlanes();
		void UpdateEntityPosition(OctreeNode* node, Entity* entity);
		void FilterEntities(Scene::entityVector& entities);
		void AssignNode(Entity* entity);
		void CullObjects();

	public:

		inline Scene::entityVector* GetViewables() { return &m_viewables; }
		inline void SetProjection(glm::mat4 proj) { m_projection = proj; }
		Octree(glm::vec3 nearTopLeft, glm::vec3 farBottomRight, float minSide, Scene::SceneGraph* scene, Camera* camera);
		void Update();
		~Octree();
	};

	void NormalizePlaneCoeff(glm::vec4& plane);
	//entityVector::iterator FindEntityInVector(Entity* entity, entityVector vector);
}

#endif // !1