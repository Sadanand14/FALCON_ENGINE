#include "Octree.h"
#include <vector>
#include <cassert>
namespace Scene
{
	OctreeNode::OctreeNode(glm::vec3 nearTopLeft, glm::vec3 farBottomRight) :m_nearTopLeft(nearTopLeft), m_farBottomRight(farBottomRight), m_parent(nullptr)
	{
		m_childNodes.reserve(10);
		m_entities.reserve(10);
	}

	OctreeNode::~OctreeNode()
	{
		for (auto& entity : m_entities) { delete entity; }
		m_entities.clear();
	}

	void OctreeNode::Subdivide(float minSide)
	{
		if ((this->m_farBottomRight.x - this->m_nearTopLeft.x) >= (2 * minSide))
		{
			glm::vec3 CentrePoint = glm::vec3((this->m_nearTopLeft + this->m_farBottomRight) / 2.0f);

			glm::vec3 vert_100 = glm::vec3(CentrePoint.x, m_nearTopLeft.y, m_nearTopLeft.z);
			glm::vec3 vert_010 = glm::vec3(m_nearTopLeft.x, CentrePoint.y, m_nearTopLeft.z);
			glm::vec3 vert_110 = glm::vec3(CentrePoint.x, CentrePoint.y, m_nearTopLeft.z);

			glm::vec3 vert_211 = glm::vec3(m_farBottomRight.x, CentrePoint.y, CentrePoint.z);
			glm::vec3 vert_121 = glm::vec3(CentrePoint.x, m_farBottomRight.y, CentrePoint.z);
			glm::vec3 vert_221 = glm::vec3(m_farBottomRight.x, m_farBottomRight.y, CentrePoint.z);

			/*this->m_childNodes.push_back(fmemory::fnew<OctreeNode>(m_nearTopLeft, CentrePoint));
			this->m_childNodes.push_back(fmemory::fnew<OctreeNode>(vert_100, vert_211));
			this->m_childNodes.push_back(fmemory::fnew<OctreeNode>(vert_010, vert_121));
			this->m_childNodes.push_back(fmemory::fnew<OctreeNode>(vert_110, vert_221));*/

			this->m_childNodes.push_back(new OctreeNode(m_nearTopLeft, CentrePoint));
			this->m_childNodes.push_back(new OctreeNode(vert_100, vert_211));
			this->m_childNodes.push_back(new OctreeNode(vert_010, vert_121));
			this->m_childNodes.push_back(new OctreeNode(vert_110, vert_221));

			glm::vec3 vert_001 = glm::vec3(m_nearTopLeft.x, m_nearTopLeft.y, CentrePoint.z);
			glm::vec3 vert_101 = glm::vec3(CentrePoint.x, m_nearTopLeft.y, CentrePoint.z);
			glm::vec3 vert_011 = glm::vec3(m_nearTopLeft.x, CentrePoint.y, CentrePoint.z);

			glm::vec3 vert_112 = glm::vec3(CentrePoint.x, CentrePoint.y, m_farBottomRight.z);
			glm::vec3 vert_212 = glm::vec3(m_farBottomRight.x, CentrePoint.y, m_farBottomRight.z);
			glm::vec3 vert_122 = glm::vec3(CentrePoint.x, m_farBottomRight.y, m_farBottomRight.z);

			/*m_childNodes.push_back(fmemory::fnew<OctreeNode>(vert_001, vert_112));
			m_childNodes.push_back(fmemory::fnew<OctreeNode>(vert_101, vert_212));
			m_childNodes.push_back(fmemory::fnew<OctreeNode>(vert_011, vert_122));
			m_childNodes.push_back(fmemory::fnew<OctreeNode>(CentrePoint, m_farBottomRight));*/

			m_childNodes.push_back(new OctreeNode(vert_001, vert_112));
			m_childNodes.push_back(new OctreeNode(vert_101, vert_212));
			m_childNodes.push_back(new OctreeNode(vert_011, vert_122));
			m_childNodes.push_back(new OctreeNode(CentrePoint, m_farBottomRight));

			m_childNodes[0]->SetParent(this);

			m_childNodes[1]->SetParent(this);

			m_childNodes[2]->SetParent(this);

			m_childNodes[3]->SetParent(this);

			m_childNodes[4]->SetParent(this);

			m_childNodes[5]->SetParent(this);

			m_childNodes[6]->SetParent(this);

			m_childNodes[7]->SetParent(this);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	Octree::Octree(glm::vec3 nearTopLeft, glm::vec3 farBottomRight, float minSide, SceneGraph* scene, Camera* camera) : m_nearTopLeft(nearTopLeft), m_farBottomRight(farBottomRight), m_scene(scene), m_camera(camera)
	{
		assert((farBottomRight.x - nearTopLeft.x) > 0.0f
			&& (nearTopLeft.y - farBottomRight.y) > 0.0f
			&& (farBottomRight.z - nearTopLeft.z) > 0.0f);

		m_rootNode = fmemory::fnew<OctreeNode>(m_nearTopLeft, m_farBottomRight);

		m_minSide = minSide;

		auto entityList = m_scene->GetEntities();
		for (unsigned int i = 0; i < entityList.size(); i++)
			entityList[i]->GetTransform()->ClearOTID();

		FilterEntities(entityList);
		m_entities = entityList;
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			AssignNode(m_entities[i]);
		}
	}

	Octree::~Octree()
	{
		//fmemory::fdelete<OctreeNode> (m_rootNode);
	}

	void Octree::AssignNode(Entity* entity)
	{
		//get bounding corners
		RenderComponent* rd = entity->GetComponent<RenderComponent>();
		boundingVector objectBounds = entity->GetComponent<RenderComponent>()->GetBounds();
		Transform* transform = entity->GetTransform();
		entity->GetTransform()->ClearOTID();

		glm::mat4 modelMatrix = entity->GetTransform()->GetModel();
		glm::vec3 temp;
		glm::vec4 result;

		//get new axis aligned boudning corners
		float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN, maxZ = FLT_MIN;

		//multiply the corners with the current model matrix
		for (unsigned short i = 0; i < objectBounds.size(); i++)
		{
			result = modelMatrix * glm::vec4(objectBounds[i], 1.0f);
			if (minX > result.x) minX = result.x;
			if (maxX < result.x) maxX = result.x;

			if (minY > result.y) minY = result.y;
			if (maxY < result.y) maxY = result.y;

			if (minZ > result.z) minZ = result.z;
			if (maxZ < result.z) maxZ = result.z;
		}

		glm::vec3 NTL = glm::vec3(minX, minY, minZ);

		glm::vec3 FBR = glm::vec3(maxX, maxY, maxZ);

		//go into a while loop that keeps checking if the children of the current node are able to contain the object completely
		//if they cant then the current node is the bounding node
		OctreeNode* node;
		node = m_rootNode;
		entity->GetTransform()->pushOTID(0);
		bool nodeFound = false, fitsInChild = false;
		while (!nodeFound)
		{
			fitsInChild = false;
			if (node->m_childNodes.empty())
			{
				node->Subdivide(m_minSide);
			}
			OctreeNodeVector* childVector = &node->m_childNodes;
			for (unsigned short i = 0; i < childVector->size(); i++)
			{
				if (CheckBounds((*childVector)[i], NTL, FBR))
				{
					transform->pushOTID(i);
					fitsInChild = true;
					node = (*childVector)[i];
					break;
				}
			}
			if (!fitsInChild) nodeFound = true;
		}

		//Assign entity to the node
		node->m_entities.push_back(entity);

		std::cout << "Node Asssigned : ";
		const IDVector* IDarr = &transform->GetOTID();
		for (unsigned int i = 0; i < transform->GetOTID().size(); i++)
		{
			std::cout << (*IDarr)[i];
		}
		std::cout << "\n";
	}

	void Octree::AddEntity(Entity* entity)
	{
		entity->GetTransform()->ClearOTID();
		AssignNode(entity);
	}

	void Octree::RemoveEntity(Entity* entity)
	{

	}

	void Octree::Update()
	{
		//const entityVector updatedEntities = m_scene->GetOctreeEntities();
		entityVector& updatedEntities = m_scene->GetOctreeEntities();
		FilterEntities(updatedEntities);

		//TODO: Add Function Call to extract plane Equations
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
				UpdateEntityPosition(FindNode(updatedEntities[i]), updatedEntities[i]);
			}
		}
	}

	void Octree::GetPlanes()
	{
		using namespace glm;
		mat4 View = m_camera->GetViewMatrix();
		mat4 VP = m_projection * View;

		vec4 xColumn = VP[0];
		vec4 yColumn = VP[1];
		vec4 zColumn = VP[2];
		vec4 wColumn = VP[3];

	}

	void Octree::UpdateEntityPosition(OctreeNode* node, Entity* entity)
	{
		//recalculate bounding box
		const boundingVector& BV = entity->GetComponent<RenderComponent>()->GetBounds();
		Transform* transform = entity->GetTransform();
		//entity->GetTransform()->ClearOTID();

		glm::mat4 modelMatrix = entity->GetTransform()->GetModel();
		glm::vec3 temp;
		glm::vec4 result;

		//get new axis aligned boudning corners
		float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN, maxZ = FLT_MIN;

		//multiply the corners with the current model matrix
		for (unsigned short i = 0; i < BV.size(); i++)
		{
			result = modelMatrix * glm::vec4(BV[i], 1.0f);
			if (minX > result.x) minX = result.x;
			if (maxX < result.x) maxX = result.x;

			if (minY > result.y) minY = result.y;
			if (maxY < result.y) maxY = result.y;

			if (minZ > result.z) minZ = result.z;
			if (maxZ < result.z) maxZ = result.z;
		}

		glm::vec3 NTL = glm::vec3(minX, minY, minZ);

		glm::vec3 FBR = glm::vec3(maxX, maxY, maxZ);

		if (!CheckBounds(node, NTL, FBR))
		{
			Transform* transform = entity->GetTransform();
			entityVector& entities = node->m_entities;
			auto entityPos = std::find(entities.begin(), entities.end(), entity);

			assert(entityPos != entities.end());
		
			entities.erase((entityPos));
			transform->popOTID();

			//go up the heirarchy to find the boudning node
			while (true)
			{
				node = node->m_parent;
				if (CheckBounds(node, NTL, FBR)) break;
				else transform->popOTID();
				if (node == m_rootNode)
				{
					m_rootNode->m_entities.push_back(entity);
					transform->pushOTID(0);
					return;
				}
			}

			//go down from there to find the lowest bounding node
			bool nodeFound = false, fitsInChild = false;
			while (!nodeFound)
			{
				fitsInChild = false;
				if (node->m_childNodes.empty())
				{
					node->Subdivide(m_minSide);
				}
				OctreeNodeVector* childVector = &node->m_childNodes;
				for (unsigned short i = 0; i < childVector->size(); i++)
				{
					if (CheckBounds((*childVector)[i], NTL, FBR))
					{
						transform->pushOTID(i);
						fitsInChild = true;
						node = (*childVector)[i];
						break;
					}
				}
				if (!fitsInChild) nodeFound = true;
			}
			node->m_entities.push_back(entity);
		}
		else 
		{
			OctreeNode* original = node;
			//check if it fits inside children
			bool nodeFound = false, fitsInChild = false;
			while (!nodeFound)
			{
				fitsInChild = false;
				if (node->m_childNodes.empty())
				{
					node->Subdivide(m_minSide);
				}
				OctreeNodeVector* childVector = &node->m_childNodes;
				for (unsigned short i = 0; i < childVector->size(); i++)
				{
					if (CheckBounds((*childVector)[i], NTL, FBR))
					{
						transform->pushOTID(i);
						fitsInChild = true;
						node = (*childVector)[i];
						break;
					}
				}
				if (!fitsInChild) nodeFound = true;
			}
			if (original != node) node->m_entities.push_back(entity);
		}
		//node->m_entities.push_back(entity);

		//std::cout << "Updated Node Asssigned : ";
		//const IDVector* IDarr = &transform->GetOTID();
		/*for (unsigned int i = 0; i < transform->GetOTID().size(); i++)
		{
			std::cout << (*IDarr)[i];
		}
		std::cout << "\n";*/
		
	}

	bool CheckBounds(OctreeNode* node, glm::vec3 NTL, glm::vec3 FBR)
	{
		glm::vec3 nodeNTL = node->m_nearTopLeft;
		glm::vec3 nodeFBR = node->m_farBottomRight;

		if (NTL.x < nodeNTL.x)
		{
			return false;
		}
		if (NTL.y > nodeNTL.y)
		{
			return false;
		}
		if (NTL.z < nodeNTL.z)
		{
			return false;
		}
		if (FBR.x > nodeFBR.x)
		{
			return false;
		}
		if (FBR.y < nodeFBR.y)
		{
			return false;
		}
		if (FBR.z > nodeFBR.z)
		{
			return false;
		}
		return true;
	}

	bool CheckEntityPosInNode(OctreeNode* node, Entity* entity)
	{
		Transform* transform = entity->GetTransform();
		glm::mat4 modelMatrix = transform->GetModel();
		float xpos = modelMatrix[3][0];
		float ypos = modelMatrix[3][1];
		float zpos = modelMatrix[3][2];
		if (
			((xpos >= node->m_nearTopLeft.x) && (xpos <= node->m_farBottomRight.x))
			&& ((ypos <= node->m_nearTopLeft.y) && (ypos >= node->m_farBottomRight.y))
			&& ((zpos >= node->m_nearTopLeft.z) && (zpos <= node->m_farBottomRight.z))
			)
			return true;
		else return false;
	}

	/*entityVector::iterator FindEntityInVector(Entity* entity, entityVector vector)
	{
		auto it = vector.begin();
		for (it; it != vector.end(); it++)
		{
			if (*it == entity) break;
		}
		if (it == vector.end())
			FL_ENGINE_ERROR("Entity Not Found in the Vector.");
		return it;
	}*/

	//void Octree::Distribute()
	//{

	//	m_rootNode->Segregate();
	//}

	void Octree::FilterEntities(entityVector& entities)
	{
		unsigned int eraseCount = 0;
		unsigned int size = entities.size();
		entityVector::iterator entityPos;
		for (unsigned int i = 0; i < size; i++)
		{
			if (!CheckEntityPosInNode(m_rootNode, entities[i]))
			{

				//entityPos = FindEntityInVector(entities[i], entities);
				entityPos = std::find(entities.begin(), entities.end(), entities[i]);
				entities.erase(entityPos);
				i--;
				eraseCount++;
				size = entities.size();
				//FindEntityInVector(entities[i], entities)
			}
		}
	}

	OctreeNode* Octree::FindNode(Entity* entity)const
	{
		const IDVector* OTID = &entity->GetTransform()->GetOTID();
		OctreeNode* temp = m_rootNode;

		for (unsigned short int i = 1; i < OTID->size(); i++)
		{
			temp = temp->m_childNodes[(*OTID)[i]];
		}

		return temp;
	}
}
