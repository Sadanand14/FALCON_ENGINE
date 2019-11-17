#include "Octree.h"
namespace Scene
{
	OctreeNode::OctreeNode(glm::vec3 nearTopLeft, glm::vec3 farBottomRight) :m_nearTopLeft(nearTopLeft), m_farBottomRight(farBottomRight), m_parent(nullptr)
	{

	}

	OctreeNode::~OctreeNode()
	{
		for (auto& entity : m_entities) { delete entity; }
		m_entities.clear();
	}

	void OctreeNode::AddEntity(Entity* entity)
	{
		m_entities.push_back(entity);
		for (unsigned int i = 0; i < m_childNodes.size(); i++)
		{
			if (CheckEntityPosInNode(m_childNodes[i], entity)) 
			{
				entity->GetTransform()->pushOTID(i);
				m_childNodes[i]->AddEntity(entity);
				break;
			}
		}
	}

	void OctreeNode::RemoveEntity(Entity* entity)
	{

	}

	void OctreeNode::Subdivide(float minSide)
	{
		if ((this->m_nearTopLeft.x - this->m_farBottomRight.x) >= (2 * minSide))
		{
			glm::vec3 CentrePoint = glm::vec3((this->m_nearTopLeft + this->m_farBottomRight) / 2.0f);

			glm::vec3 vert_100 = glm::vec3(CentrePoint.x, m_nearTopLeft.y, m_nearTopLeft.z);
			glm::vec3 vert_010 = glm::vec3(m_nearTopLeft.x, CentrePoint.y, m_nearTopLeft.z);
			glm::vec3 vert_110 = glm::vec3(CentrePoint.x, CentrePoint.y, m_nearTopLeft.z);

			glm::vec3 vert_211 = glm::vec3(m_farBottomRight.x, CentrePoint.y, CentrePoint.z);
			glm::vec3 vert_121 = glm::vec3(CentrePoint.x, m_farBottomRight.y, CentrePoint.z);
			glm::vec3 vert_221 = glm::vec3(m_farBottomRight.x, m_farBottomRight.y, CentrePoint.z);

			this->m_childNodes.push_back(fmemory::fnew<OctreeNode>(m_nearTopLeft, CentrePoint));
			this->m_childNodes.push_back(fmemory::fnew<OctreeNode>(vert_100, vert_211));
			this->m_childNodes.push_back(fmemory::fnew<OctreeNode>(vert_010, vert_121));
			this->m_childNodes.push_back(fmemory::fnew<OctreeNode>(vert_110, vert_221));

			glm::vec3 vert_001 = glm::vec3(m_nearTopLeft.x, m_nearTopLeft.y, CentrePoint.z);
			glm::vec3 vert_101 = glm::vec3(CentrePoint.x, m_nearTopLeft.y, CentrePoint.z);
			glm::vec3 vert_011 = glm::vec3(m_nearTopLeft.x, CentrePoint.y, CentrePoint.z);

			glm::vec3 vert_112 = glm::vec3(CentrePoint.x, CentrePoint.y, m_farBottomRight.z);
			glm::vec3 vert_212 = glm::vec3(m_farBottomRight.x, CentrePoint.y, m_farBottomRight.z);
			glm::vec3 vert_122 = glm::vec3(CentrePoint.x, m_farBottomRight.y, m_farBottomRight.z);

			m_childNodes.push_back(fmemory::fnew<OctreeNode>(vert_001, vert_112));
			m_childNodes.push_back(fmemory::fnew<OctreeNode>(vert_101, vert_212));
			m_childNodes.push_back(fmemory::fnew<OctreeNode>(vert_011, vert_122));
			m_childNodes.push_back(fmemory::fnew<OctreeNode>(CentrePoint, m_farBottomRight));

			m_childNodes[0]->Subdivide(minSide);
			m_childNodes[0]->SetParent(this);

			m_childNodes[1]->Subdivide(minSide);
			m_childNodes[1]->SetParent(this);

			m_childNodes[2]->Subdivide(minSide);
			m_childNodes[2]->SetParent(this);

			m_childNodes[3]->Subdivide(minSide);
			m_childNodes[3]->SetParent(this);

			m_childNodes[4]->Subdivide(minSide);
			m_childNodes[4]->SetParent(this);

			m_childNodes[5]->Subdivide(minSide);
			m_childNodes[5]->SetParent(this);

			m_childNodes[6]->Subdivide(minSide);
			m_childNodes[6]->SetParent(this);

			m_childNodes[7]->Subdivide(minSide);
			m_childNodes[7]->SetParent(this);

		}
	}

	void OctreeNode::Segregate()
	{
		if ((m_childNodes.size() == 8) && (m_entities.size() > 0))
		{
			glm::vec3 CentrePoint = glm::vec3((m_nearTopLeft + m_farBottomRight) / 2.0f);
			entityVector nearTopLeftVector, nearTopRightVector, nearBottomLeftVector, nearBottomRightVector;
			entityVector farTopLeftVector, farTopRightVector, farBottomLeftVector, farBottomRightVector;

			float xPos, yPos, zPos;
			bool close = false, top = false, left = false;
			glm::mat4 modelMatrix;
			for (unsigned int i = 0; i < m_entities.size(); i++)
			{
				Entity* entity = m_entities[i];
				modelMatrix = m_entities[i]->GetTransform()->GetModel();
				xPos = modelMatrix[1][4];
				yPos = modelMatrix[2][4];
				zPos = modelMatrix[3][4];


				if (xPos < CentrePoint.x) left = true;
				else left = false;

				if (yPos > CentrePoint.y) top = true;
				else top = false;

				if (zPos < CentrePoint.z) close = true;
				else close = false;


				if (left && top && close) { entity->GetTransform()->pushOTID(0); nearTopLeftVector.push_back(entity); }

				else if (!left && top && close) { entity->GetTransform()->pushOTID(1); nearTopRightVector.push_back(entity); }

				else if (left && !top && close) { entity->GetTransform()->pushOTID(2); nearBottomLeftVector.push_back(entity); }

				else if (!left && !top && close) { entity->GetTransform()->pushOTID(3); nearBottomRightVector.push_back(entity); }

				else if (left && top && !close) { entity->GetTransform()->pushOTID(4); farTopLeftVector.push_back(entity); }

				else if (!left && top && !close) { entity->GetTransform()->pushOTID(5); farTopRightVector.push_back(entity); }

				else if (left && !top && !close) { entity->GetTransform()->pushOTID(6); farBottomLeftVector.push_back(entity); }

				else if (!left && !top && !close) { entity->GetTransform()->pushOTID(7); farBottomRightVector.push_back(entity); }

				else FL_GAME_ERROR("Entity exists outside the octree dimensions!");
			}

			m_childNodes[0]->m_entities = nearTopLeftVector;
			m_childNodes[0]->Segregate();

			m_childNodes[1]->m_entities = nearTopRightVector;
			m_childNodes[1]->Segregate();

			m_childNodes[2]->m_entities = nearBottomLeftVector;
			m_childNodes[2]->Segregate();

			m_childNodes[3]->m_entities = nearBottomRightVector;
			m_childNodes[3]->Segregate();

			m_childNodes[4]->m_entities = farTopLeftVector;
			m_childNodes[4]->Segregate();

			m_childNodes[5]->m_entities = farTopRightVector;
			m_childNodes[5]->Segregate();

			m_childNodes[6]->m_entities = farBottomLeftVector;
			m_childNodes[6]->Segregate();

			m_childNodes[7]->m_entities = farBottomRightVector;
			m_childNodes[7]->Segregate();
		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	Octree::Octree(glm::vec3 nearTopLeft, glm::vec3 farBottomRight, float minSide, SceneGraph* scene) : m_nearTopLeft(nearTopLeft), m_farBottomRight(farBottomRight), m_scene(scene)
	{
		m_rootNode = fmemory::fnew<OctreeNode>(m_nearTopLeft, m_farBottomRight);
		m_rootNode->Subdivide(minSide);
	}



	Octree::~Octree()
	{
		delete m_rootNode;
	}

	void Octree::AddEntity(Entity* entity)
	{
		entity->GetTransform()->ClearOTID();
		m_rootNode->AddEntity(entity);
	}

	void Octree::RemoveEntity(Entity* entity)
	{

	}

	void Octree::Update()
	{
		const entityVector updatedEntities = m_scene->GetOctreeEntities();
		for (unsigned int i = 0; i < updatedEntities.size(); i++)
		{
			if (updatedEntities[i]->GetTransform()->GetOTID().empty())
			{
				if (CheckEntityPosInNode(m_rootNode, updatedEntities[i]))
				{
					AddEntity(updatedEntities[i]);
				}
			}
			else
			{
				Entity* entity = updatedEntities[i];
				UpdateEntityPosition(FindNode(entity), entity);
			}
		}
	}

	void Octree::UpdateEntityPosition(OctreeNode* node, Entity* entity)
	{
		if (!CheckEntityPosInNode(node, entity))
		{
			auto transform = entity->GetTransform();
			auto entities = node->m_entities;
			entities.erase(FindEntityInVector(entity, entities));
			transform->popOTID();

			bool nodeFound = false;
			while (!nodeFound)
			{
				node = node->m_parent;
				if (CheckEntityPosInNode(node, entity))
				{
					for (unsigned int i = 0; i < node->m_childNodes.size(); i++)
					{
						if (CheckEntityPosInNode(node->m_childNodes[i], entity))
						{
							node->m_childNodes[i]->m_entities.push_back(entity);
							transform->pushOTID(i);
							//TODO::ADD ENTITY 
							break;
						}
					}
					nodeFound = true;
				}
				else {
					entities = node->m_entities;
					entities.erase(FindEntityInVector(entity, entities));
					transform->popOTID();
				}
			}
		}
	}

	bool CheckEntityPosInNode(OctreeNode* node, Entity* entity)
	{
		Transform* transform = entity->GetTransform();
		glm::mat4 modelMatrix = transform->GetModel();
		float xpos = modelMatrix[1][4];
		float ypos = modelMatrix[2][4];
		float zpos = modelMatrix[3][4];
		if (
			((xpos > node->m_nearTopLeft.x) && (xpos < node->m_farBottomRight.x))
			&& ((ypos > node->m_nearTopLeft.y) && (ypos < node->m_farBottomRight.y))
			&& ((zpos > node->m_nearTopLeft.z) && (zpos < node->m_farBottomRight.z))
			)
			return true;
		else return false;
	}

	entityVector::iterator FindEntityInVector(Entity* entity, entityVector vector)
	{
		auto it = vector.begin();
		for (it; it != vector.end(); it++)
		{
			if (*it == entity) break;
		}
		if (it == vector.end()) FL_ENGINE_ERROR("Entity Not Found in the Vecto.r");
		return it;
	}

	void Octree::Distribute()
	{
		auto entityList = m_scene->GetEntities();
		for (unsigned int i = 0; i < entityList.size(); i++)
			entityList[i]->GetTransform()->ClearOTID();
		FilterEntities(entityList);
		m_rootNode->m_entities = entityList;
		m_rootNode->Segregate();
	}

	void Octree::FilterEntities(entityVector& entities)
	{
		for (unsigned int i = 0; i < entities.size(); i++)
		{
			if (!CheckEntityPosInNode(m_rootNode, entities[i]))
			{
				entities.erase(FindEntityInVector(entities[i], entities));
			}
		}
	}

	OctreeNode* Octree::FindNode(Entity* entity)
	{
		const IDVector OTID = entity->GetTransform()->GetOTID();
		OctreeNode* temp = m_rootNode;

		for (unsigned short int i = 0; i < OTID.size(); i++)
		{
			temp = temp->m_childNodes[OTID[i]];
		}

		if (temp == m_rootNode)
		{
			FL_ENGINE_ERROR("Could not find the object node or the octree does not have any nodes.");
			return nullptr;
		}
		else return temp;
	}
}
