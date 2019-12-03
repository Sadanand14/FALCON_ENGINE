#include "Octree.h"
#include <vector>
#include <cassert>
namespace Scene
{
	OctreeNode::OctreeNode(glm::vec3 nearTopLeft, glm::vec3 farBottomRight) :m_nearTopLeft(nearTopLeft), m_farBottomRight(farBottomRight), m_parent(nullptr)
	{
		m_childNodes.reserve(10);
		m_entities.reserve(10);
		m_centre = (m_nearTopLeft + m_farBottomRight) / 2.0f;
		float x = (m_nearTopLeft.x - m_centre.x);
		float y = (m_nearTopLeft.y - m_centre.y);
		float z = (m_nearTopLeft.z - m_centre.z);
		m_radius = glm::sqrt(x * x + y * y + z * z);
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

		m_planeArr.reserve(6);
		m_minSide = minSide;

		auto entityList = m_scene->GetRenderables();
		for (unsigned int i = 0; i < entityList.size(); i++)
			entityList[i]->GetTransform()->ClearOTID();

		FilterEntities(entityList);
		m_renderables = entityList;
		for (unsigned int i = 0; i < m_renderables.size(); i++)
		{
			AssignNode(m_renderables[i]);
		}
	}

	Octree::~Octree()
	{
		//fmemory::fdelete<OctreeNode>(m_rootNode);
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

		//extract plane equations in world Space 
		GetPlanes();

		//update all entities whose model matrices may have changed
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

		CullObjects();
	}

	void Octree::CullObjects() 
	{
		m_viewables.clear();
		bool shouldDraw = false;
		/*for (unsigned int i = 0; i < m_renderables.size(); i++) 
		{
			shouldDraw = true;
			OctreeNode* node = FindNode(m_renderables[i]);
			glm::vec3 centre= node->m_centre;
			for (unsigned int i = 0; i < m_planeArr.size(); i++) 
			{
				glm::vec4 plane = m_planeArr[i];
				if ((plane.x * centre.x + plane.y * centre.y + plane.z * centre.z + plane.w + node->m_radius) < 0) 
				{
					shouldDraw = false;
					break;
				}
			}
			if (shouldDraw)m_viewables.push_back(m_renderables[i]);
		}*/
		for (unsigned int i = 0; i < m_renderables.size(); i++) 
		{
			shouldDraw = true;
			Transform* transform = m_renderables[i]->GetTransform();
			glm::mat4 modelMatrix = transform->GetModel();
			float xpos = modelMatrix[3][0];
			float ypos = modelMatrix[3][1];
			float zpos = modelMatrix[3][2];
			for (unsigned int i = 0; i < m_planeArr.size(); i++) 
			{
				if (m_planeArr[i].x * xpos + m_planeArr[i].y * ypos + m_planeArr[i].z * zpos + m_planeArr[i].w < 0) 
				{
					shouldDraw = false;
					break;

				}
			}
			if (shouldDraw)m_viewables.push_back(m_renderables[i]);
		}
	}

	void Octree::GetPlanes()
	{
		//float zmin = -m_projection[2][3]/ m_projection[2][2];
		//float farClip = 100.0f;
		//float r = farClip / farClip - zmin;
		//m_projection[2][2] = r;
		//m_projection[2][3] = -r * zmin;
		m_planeArr.clear();
		using namespace glm;
		mat4 View = m_camera->GetViewMatrix();
		mat4 VP =  m_projection * View;
		mat4 TVP = glm::transpose(VP);
		//vec4 xColumn = VP[0];
		//vec4 yColumn = VP[1];
		//vec4 zColumn = VP[2];
		//vec4 wColumn = VP[3];

		m_planeArr.push_back(TVP[3] + TVP[0]);//Left Plane
		NormalizePlaneCoeff(m_planeArr[0]);

		m_planeArr.push_back(TVP[3] - TVP[0]);//Right Plane
		NormalizePlaneCoeff(m_planeArr[1]);

		m_planeArr.push_back(TVP[3] + TVP[1]);//Bottom Plane
		NormalizePlaneCoeff(m_planeArr[2]);

		m_planeArr.push_back(TVP[3] - TVP[1]);//Top Plane
		NormalizePlaneCoeff(m_planeArr[3]);

		m_planeArr.push_back(TVP[3] + TVP[2]);//Near Plane
		NormalizePlaneCoeff(m_planeArr[4]);

		m_planeArr.push_back(TVP[3] - TVP[2]);//Far Plane
		NormalizePlaneCoeff(m_planeArr[5]);

	/*	bool check = true;
		float total = 0;
		for (unsigned int i = 0; i < 6; i++)
		{
			if (-50*m_planeArr[i].z +  m_planeArr[i].w < 0)
				check = false;
		}
		if (check)
		{
			FL_ENGINE_TRACE("reference inside frustum");
		}
		else
		{
			FL_ENGINE_TRACE("reference outside frustum");
		}*/
	}



	void NormalizePlaneCoeff(glm::vec4& plane) 
	{
		float magnitude = glm::sqrt( plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
		plane.x /= magnitude;
		plane.y /= magnitude;
		plane.z /= magnitude;
		plane.w /= magnitude;
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
			if (original != node) 
				node->m_entities.push_back(entity);
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
