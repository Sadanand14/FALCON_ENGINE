#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <boost/container/vector.hpp>
#include <boost/container/map.hpp>
#include "EntityManager.h"

namespace Scene
{
	enum class Status { Active, Inactive };

	class SceneNode;

	typedef boost::container::vector<SceneNode*, fmemory::StackSTLAllocator<SceneNode*>> nodeVector;
	typedef boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>> entityVector;

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

	struct NodeWithOffset
	{
		SceneNode* m_sceneNode;
		unsigned int m_nextOffset;
	};

	class SceneGraph
	{
	private:
		entityVector  m_entityList, m_octreeEntityList;
		SceneNode* rootNode;
		NodeWithOffset CreateNode(rapidjson::Document& entity, unsigned int index);
	public:

		SceneGraph(const char* sceneFilePath);
		~SceneGraph();
		inline entityVector* GetEntities() { return &m_entityList; }
		inline const entityVector& GetOctreeEntities() const { return m_octreeEntityList; }

		void UpdateScene();

	};

}
#endif // !1