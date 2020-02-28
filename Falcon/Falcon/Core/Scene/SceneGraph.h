#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <Types.h>
#include "EntityManager.h"

namespace Scene
{
	/**
	* Enum class for entites to allow them to be exempted or included in the updateloop for the game.
	*/
	enum class Status { Active, Inactive };

	class SceneNode;

	typedef boost::container::vector<SceneNode*, fmemory::STLAllocator<SceneNode*>> nodeVector;
	typedef boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>> entityVector;

	/**
	* Data structure to hold the data and implementations for a Node inside the SceneGraph.
	*/
	class SceneNode
	{
		bool m_updateFlag;
		SceneNode* m_parent;
		nodeVector m_childNodes;

		glm::mat4 m_ReferenceMatrix;
		Entity* m_entity;
		Status m_status;

		inline void UpdateWorldMatrixWM(glm::mat4 matrix) { m_ReferenceMatrix = matrix; }

	public:

		SceneNode();
		SceneNode(Entity* entity);
		SceneNode(SceneNode* parent);
		SceneNode(SceneNode** childArray, unsigned int size);
		~SceneNode();

		inline void SetUpdateFlag(bool value) { m_updateFlag = value; }
		inline void SetParent(SceneNode* node) { m_parent = node; }
		inline void SetEntity(Entity* node) { m_entity = node; }

		inline SceneNode* GetParent() { return m_parent; }
		inline Entity* GetEntity() { return m_entity; }
		inline glm::mat4 GetWM() { return m_ReferenceMatrix; }


		void SetStatus(Status status);
		void AddChild(SceneNode* child);
		void RemoveChild(SceneNode* child);
		void RemoveChild(unsigned int index);

		void UpdateEntity(entityVector& vector);


		inline SceneNode* GetChildByIndex(unsigned int index) { return m_childNodes[index]; }
		inline nodeVector& GetChildren() { return m_childNodes; }
	};

	/**
	* Data structure that is explicitly used to pass data around in the function that loads the scene from json.
	*/
	struct NodeWithOffset
	{
		SceneNode* m_sceneNode;
		unsigned int m_nextOffset;
	};

	/**
	* Data Structure that holds the data and implementaion for a SceneGraph.
	*/
	class SceneGraph
	{
	private:
		entityVector  m_entityList, m_renderables, m_updatedRenderables;
		SceneNode* m_rootNode;

		NodeWithOffset CreateNode(rapidjson::Document& entity, unsigned int index, bool isReadingCar, unsigned int carIndex, RigidbodyDynamic* actor);
		void SegregateEntities();

	public:

		SceneGraph(const char* sceneFilePath);
		~SceneGraph();

		inline entityVector& GetRenderables() { return m_renderables; }
		inline entityVector* GetEntities() { return &m_entityList; }
		inline entityVector& GetOctreeEntities() { return m_updatedRenderables; }

		void UpdateScene();

	};

}
#endif // !1
