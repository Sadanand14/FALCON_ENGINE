#include "SceneGraph.h"

SceneNode::SceneNode(const char* path)
	:m_status(Status::Active), m_filePath(path), m_entity(nullptr),m_parent(nullptr),m_worldMatrix(glm::mat4())
{
}

SceneNode::SceneNode(const char* path, SceneNode* parent) 
	: m_status(Status::Active), m_filePath(path), m_entity(nullptr), m_parent(parent)
{
	m_worldMatrix = m_parent->GetWM();
}

//empty Scene Node intialization for local transform grouping
SceneNode::SceneNode(SceneNode** childArray, unsigned int size)
	: m_status(Status::Active), m_filePath(nullptr), m_entity(nullptr), m_parent(nullptr), m_worldMatrix(glm::mat4())
{
	for (unsigned int i = 0; i < size; i++)
		AddChild(childArray[i]);
}

SceneNode::~SceneNode()
{
	if (m_entity != nullptr)fmemory::fdelete<Entity>(m_entity);
}

void SceneNode::SetStatus(Status status)
{
	m_status = status;
	for (unsigned int i = 0; i < m_childNodes.size(); i++)
		m_childNodes[i]->SetStatus(status);
}

void SceneNode::LoadEntity()
{

}

void SceneNode::UpdateEntity()
{
	if (m_updateFlag) 
	{
		Transform* transform = m_entity->GetTransform();
		transform->SetRelativeSpace(m_parent->GetWM());
		transform->CheckFlag();
		m_worldMatrix = transform->GetModel();
		m_updateFlag = false;
		for (unsigned int i = 0; i < m_childNodes.size(); i++)
		{
			m_childNodes[i]->SetUpdateFlag(true);
			m_childNodes[i]->UpdateEntity();
		}

	}
}

void SceneNode::AddChild(SceneNode* node)
{
	node->SetParent(this);
	m_childNodes.push_back(node);
}

void SceneNode::RemoveChild(SceneNode* node)
{
	for (auto iterator = m_childNodes.begin(); iterator != m_childNodes.end(); iterator++) 
	{
		if (*iterator == node)
		{
			m_childNodes.erase(iterator, iterator + 1);
			return;
		}
	}
}

void SceneNode::RemoveChild(unsigned int index)
{
	unsigned int counter = 0;
	auto iterator = m_childNodes.begin() + index;
	m_childNodes.erase(iterator, iterator + 1);
}

///////////////////////////////
void SceneGraph::LoadScene()
{
}

void SceneGraph::UpdateSCene()
{
	m_rootNode->UpdateEntity();
}
