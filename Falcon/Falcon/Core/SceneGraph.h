#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <boost/container/vector.hpp>
#include <boost/container/map.hpp>
#include "EntityManager.h"

enum class Status{Active, Inactive};

class SceneNode;

typedef boost::container::vector<SceneNode*> nodeVector;

class SceneNode 
{
	const char* m_filePath;

	SceneNode* m_parent;
	nodeVector m_childNodes;
	Entity* m_entity;
	Transform m_localTransform;
	Status m_status;

public:
	SceneNode(const char* path);
	SceneNode(const char* path, SceneNode* parent);
	SceneNode(SceneNode** childArray, unsigned int size);
	~SceneNode();

	void SetStatus(Status);
	void LoadEntity();
	void UpdateEntity();
	void AddChild(SceneNode* child);
	void RemoveChild(SceneNode* child);
	void RemoveChild(unsigned int index);

	inline SceneNode* GetChildByIndex(unsigned int index) { return m_childNodes[index]; }
	inline nodeVector GetChildren() { return m_childNodes; }
};

class SceneGraph 
{
	SceneNode* rootNode;
	
	void LoadScene();
	void UpdateSCene();
};

#endif // !1