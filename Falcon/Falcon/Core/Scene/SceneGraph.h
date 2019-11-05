#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <boost/container/vector.hpp>
#include <boost/container/map.hpp>
#include "EntityManager.h"
#include <memory/fmemory.h>

enum class Status{Active, Inactive};

class SceneNode;

typedef boost::container::vector<SceneNode*,fmemory::STLAllocator<SceneNode*>> nodeVector;

class SceneNode 
{
	const char* m_filePath;

	bool m_updateFlag;
	SceneNode* m_parent;
	nodeVector m_childNodes;
	
	glm::mat4 m_worldMatrix;
	Entity* m_entity;
	Status m_status;

	inline void UpdateWorldMatrixWM(glm::mat4 matrix) { m_worldMatrix = matrix; }

public:
	SceneNode(const char* path);
	SceneNode(const char* path, SceneNode* parent);
	SceneNode(SceneNode** childArray, unsigned int size);
	~SceneNode();

	inline void SetUpdateFlag(bool value) { m_updateFlag = value; }
	inline void SetParent(SceneNode* node) { m_parent = node; }
	inline void SetEntity(Entity* node) { m_entity = node; }

	inline SceneNode* GetParent() { return m_parent; }
	inline Entity* GetEntity() { return m_entity; }
	inline glm::mat4 GetWM() { return m_worldMatrix; }


	void SetStatus(Status status);
	void AddChild(SceneNode* child);
	void RemoveChild(SceneNode* child);
	void RemoveChild(unsigned int index);

	void LoadEntity();
	void UpdateEntity();

	inline SceneNode* GetChildByIndex(unsigned int index) { return m_childNodes[index]; }
	inline nodeVector GetChildren() { return m_childNodes; }
};

class SceneGraph 
{
	SceneNode* m_rootNode;
	
	void LoadScene();
	void UpdateSCene();
};

#endif // !1