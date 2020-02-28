#include "Renderer.h"

#include <boost/shared_ptr.hpp>
#include <Memory/fmemory.h>

//Included files
//Camera
#include <Camera.h>

//Render passes
#include "RenderPass.h"
#include "MeshRenderPass.h"
#include "ParticleRenderPass.h"
#include "TransparentRenderPass.h"
#include "CanvasRenderPass.h"
#include "SkyRenderPass.h"
#include "QuadRenderPass.h"

//Events
#include <Events/PassToRenderer.h>
#include <Events/EventManager.h>
#include <Events/PassingMeshEvent.h>

//Entities
#include <EntityInterface.h>

//TODO: REMOVE
#include "Canvas.h"
#include "CanvasItems/Label.h"

RenderEventSystem* RenderEventSystem::m_instance = nullptr;

/**
*Render Event System Constructor.
*/
RenderEventSystem::RenderEventSystem()
{
	m_threadPool = ThreadPool::GetThreadPool();
	//std::cout << "RenderEventSystem intialized with address" << this << "\n";
	subscribedList.push_back(EVENT_RENDER);
	subscribedList.push_back(EVENT_DATA_TO_RENDERER);
	SubscribeToEvents();
}

void RenderEventSystem::ShutDown()
{
	delete m_instance;
}

/**
* Function to process all the events available in the event queue.
*/
void RenderEventSystem::ProcessEvents()
{
	unsigned int count = 0;
	//FL_ENGINE_WARN("eventQueue Size: {0}. \n", eventQueue.size());
	//std::cout << "eventQueue Size: " << eventQueue.size()<<"\n";
	//count++;
	while (!eventQueue.empty())
	{
		boost::shared_ptr<Event> temp = eventQueue.front();
		eventQueue.pop_front();

		//for Data Transfer Events
		if (temp->CheckCategory(EVENT_DATA_TO_RENDERER))
		{
			FL_ENGINE_ERROR("RECIEVED THE MESH DATA!!");
			boost::shared_ptr<PassToRenderer> dataEvent = boost::static_pointer_cast<PassToRenderer>(temp);
			m_skyMesh = dataEvent->GetSkyMesh();
			m_terrainMesh = dataEvent->GetTerrainMesh();
		}
	}
}

/**
 * Function to Subscribe to all the event types listed in the local subscribe list.
 */
void RenderEventSystem::SubscribeToEvents()
{
	for (unsigned int i = 0; i < subscribedList.size(); i++)
	{
		EventManager::SubscribeToEvent(this, subscribedList[i]);
	}
}

//test function
void RenderEventSystem::PrintReception()
{
	//FL_GAME_INFO("Event Executed using Job system.");
	//std::cout<<"Event Executed SuccessFully on thread :"<< boost::this_thread::get_id()<<"\n";
}
//////////////////////

/**
*Constructor for Renderer
*/
Renderer::Renderer()
{
}

/**
*Destructor for Renderer
*/
Renderer::~Renderer()
{
	fmemory::fdelete(can);
	fmemory::fdelete(l);

	for (auto pass : m_Menu_renderPasses)
	{
		CanvasRenderPass* crp = nullptr;
		crp = dynamic_cast<CanvasRenderPass*>(pass);

		if (crp)
			fmemory::fdelete(crp);
		else
			fmemory::fdelete(pass);
	}

	for (auto pass : m_Game_renderPasses)
	{
		CanvasRenderPass* crp = nullptr;
		crp = dynamic_cast<CanvasRenderPass*>(pass);

		if (crp)
			fmemory::fdelete(crp);
		else
			fmemory::fdelete(pass);
	}

	RenderEventSystem::ShutDown();
}

/**
*Initialization function for Renderer
*/
void Renderer::Init(GLFWwindow* window)
{
	m_window = window;
	m_RES = RenderEventSystem::GetInstance();
	m_RES->ProcessEvents();
}

/**
*Function to Create Buffers or Programs to be used in the next Draw cycle
*/
void Renderer::CreateDrawStates()
{
	m_RES->ProcessEvents();
	//Draw in Wireframe mode - Comment out
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	//m_RES->ProcessEvents();

	can = fmemory::fnew<Canvas>();
	can->Setup();
}



/**
*Function to Set the relevant data in the draw states.
*/
void Renderer::SetDrawStates(boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities, glm::mat4 projection)
{
	m_skyMesh = m_RES->GetSkyMesh();
	m_terrainMesh = m_RES->GetTerrainMesh();
	m_projection = projection;
	//RigidbodyDynamic* vehActor = physics::CreateDynamicRigidActor();
	std::vector < glm::vec3, fmemory::STLAllocator<glm::vec3>> temp;
	//for (u32 i = 0; i < entities->size(); i++)
	//{
	//	RenderComponent* renderComp = entities->at(i)->GetComponent<RenderComponent>();
	//	ParticleEmitterComponent* particleComp = entities->at(i)->GetComponent<ParticleEmitterComponent>();
	//	if (renderComp || particleComp)
	//	{
	//		entities->at(i)->AddComponent<PhysicsComponent>();
	//		PhysicsComponent* physComp = entities->at(i)->GetComponent<PhysicsComponent>();

	//		if (renderComp)
	//		{
	//			
	//			if (i != 1)
	//			{
	//				physComp->SetBoxCollider(5, 5, 5);
	//				physComp->SetPhysicsBodyType(entities->at(i)->GetTransform(), physics::PhysicsBodyType::ESTATIC_BODY);

	//			}
	//			else
	//			{
	//				renderComp->m_mesh->GetVertexPositionsArray(temp);
	//				physComp->SetMeshCollider(&temp[0], temp.size(), sizeof(glm::vec3));
	//				physComp->SetPhysicsBodyType(entities->at(i)->GetTransform(), physics::PhysicsBodyType::EDYNAMIC_BODY);
	//			}
	//			//renderComp->m_mesh->GetVertexPositionsArray(temp);
	//			//physComp->AddToExclusiveShape(vehActor, entities->at(i)->GetTransform(), &temp[0], temp.size(), sizeof(glm::vec3));
	//		}

	//		if (particleComp)
	//		{
	//			physComp->SetBoxCollider(5, 5, 5);
	//			physComp->SetPhysicsBodyType(entities->at(i)->GetTransform(), physics::PhysicsBodyType::ESTATIC_BODY);
	//		}
	//	}
	//}
	//physics::CreateCar(vehActor, *entities->at(0)->GetTransform());
	m_renderPasses.push_back(fmemory::fnew<MeshRenderPass>(0));
	m_renderPasses.push_back(fmemory::fnew<ParticleRenderPass>(1));
	m_renderPasses.push_back(fmemory::fnew<SkyRenderPass>(2));
	m_renderPasses.push_back(fmemory::fnew<TransparentRenderPass>(3));
	m_renderPasses.push_back(fmemory::fnew<CanvasRenderPass>(4));

	l = fmemory::fnew<Label>("Test Text");
	l->SetFlags(NK_WINDOW_DYNAMIC | NK_WINDOW_NO_SCROLLBAR);
	l->SetColor(nk_rgb(0, 0, 0));
	l->SetWrap(true);
	l->SetBounds(nk_rect(30, 30, 200, 60));
	l->SetText(std::string("This is a test"));
	static_cast<Canvas*>(can)->AddCanvasItem(l);
	m_Game_renderPasses[4]->QueueRenderable(can);
}

void Renderer::Pause_Update() 
{

}

void Renderer::Pause_Draw() 
{
	//Swap Buffers
	glfwSwapBuffers(m_window);
}

void Renderer::Menu_Update() 
{

}

void Renderer::Menu_Draw() 
{
	for (unsigned int i = 0; i < m_Menu_renderPasses.size(); ++i) 
	{
		m_Menu_renderPasses[i]->Render();
	}
	//Swap Buffers
	glfwSwapBuffers(m_window);
}


void Renderer::Ingame_Update(Camera& cam, float dt, boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities)
{
	m_RES->ProcessEvents();
	m_entities = entities;

	//for skybox
	Shader* skyShader = m_skyMesh->GetMaterial()->m_shader;
	skyShader->UseShader();
	skyShader->SetMat4("projection", m_projection);
	skyShader->SetMat4("view", cam.GetViewMatrix());

	//for terrain
	Shader* temp = m_terrainMesh->GetMaterial()->m_shader;
	temp->UseShader();
	temp->SetMat4("projection", m_projection);
	temp->SetMat4("view", cam.GetViewMatrix());


	for (unsigned int i = 0; i < m_entities->size(); ++i)
	{
		if (m_entities->at(i)->GetComponent<RenderComponent>() != nullptr)
		{
			Shader* shader = m_entities->at(i)->GetComponent<RenderComponent>()->m_mesh->GetMaterial()->m_shader;
			shader->UseShader();
			shader->SetMat4("projection", m_projection);
			shader->SetMat4("view", cam.GetViewMatrix());
		}

		if (m_entities->at(i)->GetComponent<ParticleEmitterComponent>() != nullptr)
		{
			Shader* shader = m_entities->at(i)->GetComponent<ParticleEmitterComponent>()->m_particle->GetMaterial()->m_shader;
			shader->UseShader();
			shader->SetMat4("projection", m_projection);
			shader->SetMat4("view", cam.GetViewMatrix());
			shader->SetVec3("camPos", cam.m_Position);
		}
	}
}

void Renderer::Ingame_Draw(Camera& cam)
{
	glDepthMask(true);
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_skyMesh != nullptr)
	{
		m_skyMesh->AddWorldMatrix(glm::mat4(0.0f));
		m_Game_renderPasses[2]->QueueRenderable(m_skyMesh);
	}


	m_terrainMesh->AddWorldMatrix(glm::mat4(1.0f));
	m_Game_renderPasses[0]->QueueRenderable(m_terrainMesh);
	boost::container::flat_map<float, int> distanceEntityMap;
	for (u32 i = 0; i < m_entities->size(); i++)
	{
		Transform* trans = m_entities->at(i)->GetTransform();

		RenderComponent* rc = m_entities->at(i)->GetComponent<RenderComponent>();
		if (rc)
		{
			Mesh* m = rc->m_mesh;

			if (!m->GetTransparent())
			{
				m->AddWorldMatrix(trans->GetModel());
				m_Game_renderPasses[0]->QueueRenderable(m);
			}

			else
			{
				float dist = glm::distance(trans->GetRelativePosition(), cam.m_Position);
				distanceEntityMap[dist] = i;
			}
		}

		ParticleEmitterComponent* pec = m_entities->at(i)->GetComponent<ParticleEmitterComponent>();
		if (pec)
		{
			Particle* p = pec->m_particle;
			p->SetWorldMatrix(trans->GetModel());
			for (auto it = pec->m_particleBuffer.begin(); it != pec->m_particleBuffer.end(); it++)
				p->AddParticleData(*it);
			m_Game_renderPasses[1]->QueueRenderable(p);
		}
	}

	uint32_t count = 0;
	//Transparent meshes
	for (auto it = distanceEntityMap.rbegin(); it != distanceEntityMap.rend(); it++)
	{
		RenderComponent* rc = m_entities->at(it->second)->GetComponent<RenderComponent>();
		rc->m_mesh->AddWorldMatrix(m_entities->at(it->second)->GetTransform()->GetModel());
		count++;

		auto next = it;
		next++;

		if (next != distanceEntityMap.rend())
		{
			if (m_entities->at(next->second)->GetComponent<RenderComponent>()->m_mesh != rc->m_mesh)
			{
				m_Game_renderPasses[3]->QueueRenderable(rc->m_mesh);
				static_cast<TransparentRenderPass*>(m_Game_renderPasses[3])->AddCountAndOffset(count, rc->m_mesh->GetWorldMatrixAmount() - count);
				count = 0;
			}

			else
				continue;
		}

		else
		{
			m_Game_renderPasses[3]->QueueRenderable(rc->m_mesh);
			static_cast<TransparentRenderPass*>(m_Game_renderPasses[3])->AddCountAndOffset(count, rc->m_mesh->GetWorldMatrixAmount() - count);
			count = 0;
		}
	}

	for (u32 i = 0; i < m_Game_renderPasses.size(); i++)
	{
		m_Game_renderPasses[i]->Render();
	}

	//Swap Buffers
	glfwSwapBuffers(m_window);
}