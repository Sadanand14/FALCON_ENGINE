#include "SceneGraph.h"

namespace Scene
{

	SceneNode::SceneNode()
		: m_updateFlag(false), m_status(Status::Active), m_entity(nullptr), m_parent(nullptr), m_ReferenceMatrix(glm::mat4(1.0f))
	{
		//m_childNodes.reserve(10);
	}

	SceneNode::SceneNode(Entity* entity)
		: m_updateFlag(false), m_status(Status::Active), m_entity(entity), m_parent(nullptr), m_ReferenceMatrix(glm::mat4(1.0f))
	{
		//m_childNodes.reserve(10);
	}

	SceneNode::SceneNode(SceneNode* parent)
		: m_updateFlag(true), m_status(Status::Active), m_entity(nullptr), m_parent(parent)
	{
		//m_childNodes.reserve(10);
		m_ReferenceMatrix = m_parent->GetWM();
	}

	//empty Scene Node intialization for local transform grouping
	SceneNode::SceneNode(SceneNode** childArray, unsigned int size)
		: m_updateFlag(false), m_status(Status::Active), m_entity(nullptr), m_parent(nullptr), m_ReferenceMatrix(glm::mat4(1.0f))
	{
		//m_childNodes.reserve(10);
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

	void SceneNode::UpdateEntity(entityVector& vector)
	{
		Transform* transform = m_entity->GetTransform();
		if (m_updateFlag || transform->GetFlag())
		{
			transform->SetRelativeSpace(m_parent->GetWM());
			transform->CheckFlag();
			m_ReferenceMatrix = transform->GetModel();
			if (m_entity->GetComponent<RenderComponent>() != nullptr && m_status == Status::Active) vector.push_back(m_entity);
			m_updateFlag = false;
			for (unsigned int i = 0; i < m_childNodes.size(); i++)
			{
				m_childNodes[i]->SetUpdateFlag(true);
				m_childNodes[i]->UpdateEntity(vector);
			}
		}
	}

	void SceneNode::AddChild(SceneNode* node)
	{
		node->SetParent(this);
		node->m_updateFlag = true;
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

	/////////////////////////////////////////////////////////
	NodeWithOffset SceneGraph::CreateNode(rapidjson::Document& doc, unsigned int index)
	{
		const rapidjson::Value& world = doc["entities"];
		//Get position, rotation, and scale
		const rapidjson::Value& pos = world[index]["pos"];
		glm::vec3 position;
		for (rapidjson::SizeType j = 0; j < 3; j++)
		{
			position[j] = pos[j].GetDouble();
		}

		const rapidjson::Value& rot = world[index]["rot"];
		glm::quat rotation;
		for (rapidjson::SizeType j = 0; j < 4; j++)
		{
			rotation[j] = rot[j].GetDouble();
		}

		const rapidjson::Value& sca = world[index]["scale"];
		glm::vec3 scale;
		for (rapidjson::SizeType j = 0; j < 3; j++)
		{
			scale[j] = sca[j].GetDouble();
		}

		const char* objTemplate = NULL;
		if (world[index].HasMember("template"))
		{
			const rapidjson::Value& objPath = world[index]["template"];
			objTemplate = objPath.GetString();
		}
		else
		{
			FL_ENGINE_ERROR(" NO OBJECT TEMPLATE PROVIDED FOR ENTITY!!");
		}

		Entity* temp = EntityManager::CreateEntity(objTemplate, position, rotation, scale);
		m_entityList.push_back(temp);
		SceneNode* newNode = new SceneNode(temp);


		NodeWithOffset returnValue;
		if (world[index].HasMember("childCount"))
		{
			const rapidjson::Value& children = world[index]["childCount"];
			unsigned int loopCount = children.GetUint();

			unsigned int readIndex = ++index;

			//LoadChildren Recursively
			NodeWithOffset childNodeStuff;
			for (rapidjson::SizeType j = 0; j < loopCount; j++)
			{
				childNodeStuff = CreateNode(doc, readIndex);
				newNode->AddChild(childNodeStuff.m_sceneNode);
				readIndex = childNodeStuff.m_nextOffset;
			}
			returnValue.m_nextOffset = readIndex;
		}
		else
		{
			FL_ENGINE_ERROR("No childCount Detected!!");
		}


		returnValue.m_sceneNode = newNode;

		return returnValue;
	}



	SceneGraph::SceneGraph(const char* sceneFilePath)
	{
		m_entityList.reserve(10);
		m_octreeEntityList.reserve(10);
		rootNode = new SceneNode();
		
		//Get file data
		char* json = nullptr;
		int32_t size;
		std::ifstream jsonFile(sceneFilePath, std::ios::in | std::ios::ate | std::ios::binary);
		if (jsonFile.is_open()) {
			size = jsonFile.tellg();
			jsonFile.seekg(std::ios::beg);
			json = fmemory::fnew_arr<char>(size + 1);
			jsonFile.read(json, size);
			json[size] = 0;
			jsonFile.close();
		}

		//Start json doc
		rapidjson::Document doc;
		doc.Parse(json);
		fmemory::fdelete<char>(json);

		//Check if JSON file is not valid
		if (!doc.IsObject())
		{
			FL_ENGINE_ERROR("ERROR::{0} is not a valid JSON file", sceneFilePath);
			return;
		}

		//Check if JSON file has an entities array
		if (doc.HasMember("entities"))
		{
			unsigned int nextEntityOffset = 0;

			//Read entities
			rapidjson::Value& world = doc["entities"];
			NodeWithOffset temp;

			//Load all entities into the scene
			while (nextEntityOffset < world.Size())
			{
				temp = CreateNode(doc, nextEntityOffset);
				rootNode->AddChild(temp.m_sceneNode);
				nextEntityOffset = temp.m_nextOffset;
			}
		}
	}

	SceneGraph::~SceneGraph()
	{

	}

	void SceneGraph::UpdateScene()
	{
		m_octreeEntityList.clear();
		nodeVector* childArr = &rootNode->GetChildren();
		for (unsigned int i = 0; i < childArr->size(); i++)
			(*childArr)[i]->UpdateEntity(m_octreeEntityList);
	}
}
