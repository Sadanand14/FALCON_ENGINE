#include "Octree.h"
#include <vector>
#include <cassert>
namespace Rendering
{
	unsigned int  OctreeNode::m_nodeCount = 0;

	/**
	* Octree Node Constructor function
	*
	*@param[in] the corner point of one of the cuboidal space covered in the scene by the octreenode.
	*@param[in] the diagonally opposite corner of the octreenode.
	*/
	OctreeNode::OctreeNode(glm::vec3 nearTopLeft, glm::vec3 farBottomRight) :m_nearTopLeft(nearTopLeft), m_farBottomRight(farBottomRight), m_parent(nullptr)
	{
		//m_childNodes.reserve(10);
		//m_entities.reserve(10);
		m_centre = (m_nearTopLeft + m_farBottomRight) / 2.0f;
		float x = (m_nearTopLeft.x - m_centre.x);
		float y = (m_nearTopLeft.y - m_centre.y);
		float z = (m_nearTopLeft.z - m_centre.z);
		m_radius = glm::sqrt(x * x + y * y + z * z);
	}

	/**
	* Octree Node Destructor
	*/

	OctreeNode::~OctreeNode()
	{
		//TODO:: change this to fdelete de-allocation
		for (auto& node : m_childNodes) { delete node; }
		m_childNodes.clear();
		m_entities.clear();
	}

	/**
	* Function that divides the space covered by the octree node into 8 octants owned by 8 new childnodes.
	*@param[in] Minimum possible size of an octreenode.
	*/
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
			this->m_childNodes.reserve(8);

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

			m_nodeCount += 8;
			FL_ENGINE_ERROR("NodeCount : {0}", m_nodeCount);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////

	/**
	* Main Constructor method for Octree.
	*
	*@param[in] The corner point of one of the cuboidal space covered in the scene by the octree.
	*@param[in] The diagonally opposite corner of the octree.
	*@param[in] Pointer to the main SceneGraph managing the scene.
	*@param[in] Pointer to the Current viewing Camera
	*/
	Octree::Octree(glm::vec3 nearTopLeft, glm::vec3 farBottomRight, float minSide, Scene::SceneGraph* scene) : m_nearTopLeft(nearTopLeft), m_farBottomRight(farBottomRight), m_scene(scene)
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

		defaultVolume = { glm::vec3(-1.0f,  1.0f, -1.0f),
			glm::vec3( 1.0f,  1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3( 1.0f, -1.0f, -1.0f),

			glm::vec3(-1.0f,  1.0f,  1.0f),
			glm::vec3( 1.0f,  1.0f,  1.0f),
			glm::vec3(-1.0f, -1.0f,  1.0f),
			glm::vec3( 1.0f, -1.0f,  1.0f) };
	}


	/**
	* Default Destructor for the Octree.
	*/
	Octree::~Octree()
	{
		fmemory::fdelete<OctreeNode>(m_rootNode);
	}

	/**
	* Function that assigns a node in the octree to the entity it recieves as argument, based on its position in scene.
	*
	*@param[in] pointer to the entity that needs to be assigned a node.
	*/
	void Octree::AssignNode(Entity* entity)
	{
		entity->GetTransform()->ClearOTID();

		//get bounding corners
		boundingVector* objectBounds;

		RenderComponent* rd = entity->GetComponent<RenderComponent>();

		if(rd)
			objectBounds = rd->GetBounds();

		else
			objectBounds = &defaultVolume;

		Transform* transform = entity->GetTransform();
		entity->GetTransform()->ClearOTID();

		glm::mat4 modelMatrix = entity->GetTransform()->GetModel();
		glm::vec3 temp;
		glm::vec4 result;

		//get new axis aligned boudning corners
		float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX, maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;

		//multiply the corners with the current model matrix
		for (unsigned short i = 0; i < objectBounds->size(); i++)
		{
			result = modelMatrix * glm::vec4(objectBounds->at(i), 1.0f);
			if (minX > result.x)
				minX = result.x;
			if (maxX < result.x)
				maxX = result.x;

			if (minY > result.y)
				minY = result.y;
			if (maxY < result.y)
				maxY = result.y;

			if (minZ > result.z)
				minZ = result.z;
			if (maxZ < result.z)
				maxZ = result.z;
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

		//std::cout << "Node Asssigned : ";
		const IDVector* IDarr = &transform->GetOTID();
		for (unsigned int i = 0; i < transform->GetOTID().size(); i++)
		{
			//std::cout << (*IDarr)[i];
		}
		//std::cout << "\n";
	}

	/**
	* Main Update Function for Octree.
	* Checks for moved entities and reasssigns them to a new node if necessary.
	* Assigns nodes to entities entering the octree space.
	* Extracts viewing plane equations for the current frame
	* Culls objects based on whether their nodes intersect with the viewing plane.
	*/
	void Octree::Update()
	{
		//const entityVector updatedEntities = m_scene->GetOctreeEntities();
		Scene::entityVector& updatedEntities = m_scene->GetOctreeEntities();
		FilterEntities(updatedEntities);



		//update all entities whose model matrices may have changed
		for (unsigned int i = 0; i < updatedEntities.size(); i++)
		{
			if (updatedEntities[i]->GetTransform()->GetOTID().empty())
			{
				if (CheckEntityPosInNode(m_rootNode, updatedEntities[i]))
				{
					AssignNode(updatedEntities[i]);
				}
			}
			else
			{
				UpdateEntityPosition(FindNode(updatedEntities[i]), updatedEntities[i]);
			}
		}

		//extract plane equations in world Space
		GetPlanes();

		CullObjects();
	}


	/**
	* Function that filters objects for the rendering engine based on whether they are being seen on the camera or not.
	*/
	void Octree::CullObjects()
	{
		m_viewables.clear();
		bool shouldDraw = false;
		for (unsigned int i = 0; i < m_renderables.size(); i++)
		{
			shouldDraw = true;
			OctreeNode* node = FindNode(m_renderables[i]);
			glm::vec3 centre = node->m_centre;
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
		}
		/*for (unsigned int i = 0; i < m_renderables.size(); i++)
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
		}*/
	}

	/**
	* Function that extracts the viewing plane equations.
	*/
	void Octree::GetPlanes()
	{
		//float zmin = -m_projection[2][3]/ m_projection[2][2];
		//float farClip = 100.0f;
		//float r = farClip / farClip - zmin;
		//m_projection[2][2] = r;
		//m_projection[2][3] = -r * zmin;
		m_planeArr.clear();
		using namespace glm;
		static mat4 View;
		static mat4 VP;
		static mat4 TVP;
		View = m_view;
		VP = m_projection * View;
		TVP = glm::transpose(VP);
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


	/**
	* Non-member function for normalizing plane equations
	*/
	void NormalizePlaneCoeff(glm::vec4& plane)
	{
		static float magnitude;
		magnitude = glm::sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
		plane.x /= magnitude;
		plane.y /= magnitude;
		plane.z /= magnitude;
		plane.w /= magnitude;
	}

	/**
	*Function that checks the provided entity for updated positions and reassigns node in the tree if required.
	*
	*@param[in] Pointer the node that the provided entity belongs to.
	*@param[in] Pointer to the entity contained by the node provided.
	*/
	void Octree::UpdateEntityPosition(OctreeNode* node, Entity* entity)
	{
		static bool udpated = false;
		//recalculate bounding box
		static boundingVector* BV;

		static RenderComponent* rd;
		rd = entity->GetComponent<RenderComponent>();

		if(rd)
			BV = rd->GetBounds();

		else
			BV = &defaultVolume;

		static Transform* transform;
		transform = entity->GetTransform();
		//entity->GetTransform()->ClearOTID();

		static glm::mat4 modelMatrix;
		modelMatrix = entity->GetTransform()->GetModel();
		//static glm::vec3 temp;
		static glm::vec4 result;

		//get new axis aligned boudning corners
		float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX, maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;

		//multiply the corners with the current model matrix
		for (unsigned short i = 0; i < BV->size(); i++)
		{
			result = modelMatrix * glm::vec4(BV->at(i), 1.0f);
			if (minX > result.x) minX = result.x;
			if (maxX < result.x) maxX = result.x;

			if (minY > result.y) minY = result.y;
			if (maxY < result.y) maxY = result.y;

			if (minZ > result.z) minZ = result.z;
			if (maxZ < result.z) maxZ = result.z;
		}

		static glm::vec3 NTL;
		NTL = glm::vec3(minX, minY, minZ);

		static glm::vec3 FBR;
		FBR = glm::vec3(maxX, maxY, maxZ);

		static bool boundaryCheck = false;
		boundaryCheck = CheckBounds(node, NTL, FBR);

		if (!boundaryCheck && node!=m_rootNode)
		{
			Transform* transform = entity->GetTransform();
			Scene::entityVector& entities = node->m_entities;
			auto entityPos = std::find(entities.begin(), entities.end(), entity);

			assert(entityPos != entities.end());

			entities.erase((entityPos));
			transform->popOTID();

			//FL_ENGINE_WARN("Going up!!");
			////go up the heirarchy to find the boudning node
			while (true)
			{
				
				node = node->m_parent;
				if (CheckBounds(node, NTL, FBR)) break;
				else transform->popOTID();
				if (node == m_rootNode)
				{
					m_rootNode->m_entities.push_back(entity);
					transform->pushOTID(0);
					udpated = true;
					return;
				}
			}

			//FL_ENGINE_WARN("Going Down!!");
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
						udpated = true;
						break;
					}
				}
				if (!fitsInChild) nodeFound = true;
			}
			node->m_entities.push_back(entity);
		}
		else if(boundaryCheck)
		{
			OctreeNode* original = node;
			//FL_ENGINE_ERROR("Going Down!!");
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
						udpated = true;
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

		//if (udpated)
		//{
		//	udpated = false;
		//	std::cout << "Updated Node Asssigned : ";
		//	const IDVector* IDarr = &transform->GetOTID();
		//	for (unsigned int i = 0; i < transform->GetOTID().size(); i++)
		//	{
		//		std::cout << (*IDarr)[i];
		//	}
		//	std::cout << "\n";
		//}
	}

	/**
	* Non-Member Function that checks if the diagonally opposite corners of a bounding box are within the bounds of the provided octreenode.
	*
	*@param[in] Pointer to the Octreenode used as reference.
	*@param[in] The corner point of the bounding box.
	*@param[in] The diagonally opposite corner point of the bounding box.
	*
	*@param[out] boolean value indicating if the test was positive or negative (false if outside, true if inside).
	*/
	bool CheckBounds(OctreeNode* node, glm::vec3 NTL, glm::vec3 FBR)
	{
		static glm::vec3 nodeNTL;
		nodeNTL = node->m_nearTopLeft;
		static glm::vec3 nodeFBR;
		nodeFBR = node->m_farBottomRight;

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

	/**
	* Non-member Function that checks if the provided entity
	*
	*@param[in] Pointer to the OctreeNode.
	*@param[in] Pointer to the enitity.
	*
	*@param[out] boolean value indicating if the test was positive or negative (false if outside, true if inside).
	*/
	bool CheckEntityPosInNode(OctreeNode* node, Entity* entity)
	{
		static Transform* transform;
		transform = entity->GetTransform();

		static glm::mat4 modelMatrix;
		modelMatrix = transform->GetModel();

		static float xpos;
		xpos= modelMatrix[3][0];
		static float ypos;
		ypos = modelMatrix[3][1];
		static float zpos;
		zpos = modelMatrix[3][2];

		if (
			((xpos >= node->m_nearTopLeft.x) && (xpos <= node->m_farBottomRight.x))
			&& ((ypos <= node->m_nearTopLeft.y) && (ypos >= node->m_farBottomRight.y))
			&& ((zpos >= node->m_nearTopLeft.z) && (zpos <= node->m_farBottomRight.z))
			)
			return true;
		else return false;
	}

	/**
	* Function that filters out all entities that are not present inside the bounding space of the octree.
	*/
	void Octree::FilterEntities(Scene::entityVector& entities)
	{
		static unsigned int size;
		size = entities.size();
		static Scene::entityVector::iterator entityPos;

		for (unsigned int i = 0; i < size; i++)
		{
			if (!CheckEntityPosInNode(m_rootNode, entities[i]))
			{

				//entityPos = FindEntityInVector(entities[i], entities);
				entityPos = std::find(entities.begin(), entities.end(), entities[i]);
				entities.erase(entityPos);
				i--;
				size = entities.size();
				//FindEntityInVector(entities[i], entities)
			}
		}
	}

	/**
	* Function that returns the assigned node of the Octree to the provided entity.
	*
	*@param[in] Pointer to the entity.
	*/
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
