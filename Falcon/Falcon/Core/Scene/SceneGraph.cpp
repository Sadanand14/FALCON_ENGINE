#include "SceneGraph.h"
#include <Events/PassToRenderer.h>
#include <Events/EventManager.h>
#include <Events/CarEvent.h>
#include <Physics/vehicle/Vehicle.h>
#include <Physics/CarSystem.h>

namespace Scene
{
	/**
	* One of the constructors for a Node of the SceneGraph. Creates an Empty Node.
	*/
	SceneNode::SceneNode()
		: m_updateFlag(false), m_status(Status::Active), m_entity(nullptr), m_parent(nullptr), m_ReferenceMatrix(glm::mat4(1.0f))
	{
		m_childNodes.reserve(10);
	}

	/**
	* One of the constructors for a Node of the SceneGraph.Creates a Node for the enity provided.
	*
	*@param[in] Pointer to the entity.
	*/
	SceneNode::SceneNode(Entity* entity)
		: m_updateFlag(false), m_status(Status::Active), m_entity(entity), m_parent(nullptr), m_ReferenceMatrix(glm::mat4(1.0f))
	{
		m_childNodes.reserve(10);
	}

	/**
	* One of the constructors for a Node of the SceneGraph. Creates an empty childnode for the parent node provided.
	*
	*@param[in] The pointer to the parent node.
	*/
	SceneNode::SceneNode(SceneNode* parent)
		: m_updateFlag(true), m_status(Status::Active), m_entity(nullptr), m_parent(parent)
	{
		m_childNodes.reserve(10);
		m_ReferenceMatrix = m_parent->GetWM();
	}

	/**
	* One of the constructors for a Node of the SceneGraph. Creates a node that becomes a praent to all the nodes in the provided node array.
	*
	*@param[in] The pointer to array of Nodes.
	*@param[in] The size of the array.
	*/
	SceneNode::SceneNode(SceneNode** childArray, unsigned int size)
		: m_updateFlag(false), m_status(Status::Active), m_entity(nullptr), m_parent(nullptr), m_ReferenceMatrix(glm::mat4(1.0f))
	{
		//m_childNodes.reserve(10);
		for (unsigned int i = 0; i < size; i++)
			AddChild(childArray[i]);
	}

	/**
	* Main Destructor for a SceneNode.
	*/
	SceneNode::~SceneNode()
	{
		if (m_entity != nullptr)//fmemory::fdelete<Entity>(m_entity);
			delete m_entity;
		for (unsigned int i = 0; i < m_childNodes.size(); ++i)
		{
			//fmemory::fdelete<SceneNode>(m_childNodes[i]);
			delete m_childNodes[i];
		}
	}

	/**
	* Function that assigns the provided status to the node.
	*/
	void SceneNode::SetStatus(Status status)
	{
		m_status = status;
		for (unsigned int i = 0; i < m_childNodes.size(); i++)
			m_childNodes[i]->SetStatus(status);
	}

	/**
	* Function that Updates the node and calls update on each childnode if needed.
	* It primarily checks for any updates in the transformation matrix of its parent node as well as the entity of this node and updates based on that.
	*
	* @param[in] a vector that holds all the entities that were updated during the current loop by the scenegraph and also need to be rendered. Required for processing the octree update.
	*/
	void SceneNode::UpdateEntity(entityVector& vector)
	{
		Transform* transform = m_entity->GetTransform();
		if (m_updateFlag || transform->GetFlag())
		{
			transform->SetRelativeSpace(m_parent->GetWM());
			transform->CheckFlag();
			m_ReferenceMatrix = transform->GetModel();
			if ((m_entity->GetComponent<RenderComponent>() != nullptr || m_entity->GetComponent<ParticleEmitterComponent>() != nullptr) && m_status == Status::Active) vector.push_back(m_entity);
			m_updateFlag = false;
			for (unsigned int i = 0; i < m_childNodes.size(); i++)
			{
				m_childNodes[i]->SetUpdateFlag(true);
				m_childNodes[i]->UpdateEntity(vector);
			}
		}
	}

	/**
	* Function that adds the node provided as a child to this Scene node.
	*
	* @param[in] Pointer to the node to be added as a child.
	*/
	void SceneNode::AddChild(SceneNode* node)
	{
		node->SetParent(this);
		node->m_updateFlag = true;
		m_childNodes.push_back(node);
	}

	/**
	* Removes the provided node from the list of child nodes of this scene node.
	*
	* @param[in] Pointer to the node to be removed as a child.
	*/
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

	/**
	* Removes the node at the provided index in the child array as a child.
	*
	* @param[in] The index of the child to be discarded in the child array.
	*/
	void SceneNode::RemoveChild(unsigned int index)
	{
		unsigned int counter = 0;
		auto iterator = m_childNodes.begin() + index;
		m_childNodes.erase(iterator, iterator + 1);
	}

	/////////////////////////////////////////////////////////

	/**
	* Function that Creates a node and also instructs entity manager to create an entity based on the data read from the rapidjson document provided.
	*
	* @param[in] The document containing the data for all the enitities in this scene.
	* @param[in] The index for the entity to be read in the document.
	* @param[in] boolean specifying if we're reading a car or not
	* @param[in] Index of the car in vehicle
	* @param[in] Pointer to rigidbody, null for the common entities, specify pointer for the car.
	* 
	* @param[out] The data necessary to be relayed back for continuing to load the rest of the scene.
	*/
	NodeWithOffset SceneGraph::CreateNode(rapidjson::Document& doc, unsigned int index, bool isReadingCar, unsigned int carIndex,RigidbodyDynamic* actor)
	{
		const rapidjson::Value& world = (!isReadingCar)?doc["entities"]: doc["vehicles"][carIndex];
		//Get position, rotation, and scale
		const rapidjson::Value& name = world[index]["name"];
		FL_ENGINE_ERROR("Currently Loading: {0}", name.GetString());
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

		Entity* temp = EntityManager::CreateEntity(objTemplate, position, rotation, scale,actor);
		m_entityList.push_back(temp);

		SceneNode* newNode = new SceneNode(temp);
		//SceneNode* newNode = fmemory::fnew<SceneNode>(temp);

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
				childNodeStuff = CreateNode(doc, readIndex,isReadingCar,carIndex, actor);
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

	void SceneGraph::CreateEntityNode(rapidjson::Document& doc, unsigned int index, nodeVector& vector, int* temp)
	{
		FL_ENGINE_WARN("THREAD RUNNING with {0}", index);
	}

	/**
	* Function that filters the entities in the scene based on their components.
	*/
	void SceneGraph::SegregateEntities()
	{
		m_renderables.clear();
		//segregate all renderable objects into one vector list
		for (unsigned int i = 0; i < m_entityList.size(); i++)
		{
			if (m_entityList[i]->GetComponent<RenderComponent>() != nullptr || m_entityList[i]->GetComponent<ParticleEmitterComponent>() != nullptr)
			{
				m_renderables.push_back(m_entityList[i]);
			}
		}
	}

	/**
	* Main Contructor For SceneGraph. Loads the data and from the provided Json Scene file and constructs the scene.
	*
	* @param[in] Path to the scene file.
	*/
	SceneGraph::SceneGraph(const char* sceneFilePath)
	{
		m_entityList.reserve(10);
		m_updatedRenderables.reserve(10);
		//m_rootNode = fmemory::fnew<SceneNode>();
		m_rootNode = new SceneNode();
		//m_threads = ThreadPool::GetThreadPool();

		//Get file data
		char* json = nullptr;
		int32_t size;
		std::ifstream jsonFile(sceneFilePath, std::ios::in | std::ios::ate | std::ios::binary);
		if (jsonFile.is_open()) {
			size = jsonFile.tellg();
			jsonFile.seekg(std::ios::beg);
			//json = fmemory::fnew_arr<char>(size + 1);
			json = new char[size + 1];
			jsonFile.read(json, size);
			json[size] = 0;
			jsonFile.close();
		}

		//Start json doc
		rapidjson::Document doc;
		doc.Parse(json);
		//fmemory::fdelete<char>(json);
		delete[] json;

		//Check if JSON file is not valid
		if (!doc.IsObject())
		{
			FL_ENGINE_ERROR("ERROR::{0} is not a valid JSON file", sceneFilePath);
			return;
		}

		Mesh *terrainMesh = nullptr, *skyMesh = nullptr;

		if (doc.HasMember("terrain"))
		{
			terrainMesh = AssetManager::LoadTerrain(doc["terrain"].GetString());

			//std::cout << "Terrain Loaded";
		}


		//check for Skybox
		if (doc.HasMember("sky"))
		{
			rapidjson::Value& sky = doc["sky"];
			skyMesh = AssetManager::GetSky(sky.GetString());
		}

		if (skyMesh != nullptr || terrainMesh != nullptr)
		{
			EventManager::PushEvent(boost::make_shared<PassToRenderer>(skyMesh, terrainMesh), EVENT_DATA_TO_RENDERER);
		}

		//Check if JSON file has an entities array
		unsigned int nextEntityOffset = 0;
		if (doc.HasMember("entities"))
		{

			//Read entities
			rapidjson::Value& world = doc["entities"];
			NodeWithOffset temp;

			unsigned int size = world.Size();


			FL_ENGINE_ERROR("Loaded Size : {0}", m_entityList.size());

			//m_threads->execute_task();
			//Load all entities into the scene
			while (nextEntityOffset < world.Size())
			{
				temp = CreateNode(doc, nextEntityOffset,false, 0, nullptr);
				m_rootNode->AddChild(temp.m_sceneNode);
				nextEntityOffset = temp.m_nextOffset;
			}
		}

		if (doc.HasMember("vehicles"))
		{
			rapidjson::Value& vehicles = doc["vehicles"];

			FL_ENGINE_ERROR("Cound of vehicles = {0}", vehicles.Size());

			for (unsigned itr = 0; itr < vehicles.Size(); ++itr)
			{
				rapidjson::Value& car = doc["vehicles"][itr];
				NodeWithOffset temp;
				RigidbodyDynamic* vehActor = physics::CreateDynamicRigidActor();
				unsigned looper = 0;
				const rapidjson::Value& user_car_flag = doc["vehicles"][itr][looper]["user_car"];
				bool is_user_car = user_car_flag.GetBool();

				for (looper = 1; looper < car.Size();++looper)
				{
					temp = CreateNode(doc, looper,true, itr,vehActor);
					m_rootNode->AddChild(temp.m_sceneNode);
					nextEntityOffset = temp.m_nextOffset;
				}
				physics::vehicle::Car* vehicle = physics::vehicle::CreateCar(vehActor, *m_entityList[m_entityList.size()-1]->GetTransform());
				EventManager::PushEvent(boost::make_shared<CarEvent>(vehicle, is_user_car), EVENT_CAR_CREATED);
			}

			CarSystem::Update();
		}

		SegregateEntities();
	}

	/**
	*	Main Destructor for the SceneGarph.
	*/
	SceneGraph::~SceneGraph()
	{
		//nodeVector* childArr = &m_rootNode->GetChildren();
		//fmemory::fdelete<SceneNode>(m_rootNode);
		delete m_rootNode;
	}

	/**
	* Update function for the Scene Graph. Iterates through all direct children of the rootnode and updates them.
	*/
	void SceneGraph::UpdateScene()
	{
		m_updatedRenderables.clear();
		nodeVector* childArr = &m_rootNode->GetChildren();
		for (unsigned int i = 0; i < childArr->size(); i++)
			(*childArr)[i]->UpdateEntity(m_updatedRenderables);
	}
}
