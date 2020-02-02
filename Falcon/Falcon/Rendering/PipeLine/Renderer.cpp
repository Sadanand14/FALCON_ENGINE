#include "Renderer.h"

//Included files
//Camera
#include <Camera.h>

//Render passes
#include "RenderPass.h"
#include "MeshRenderPass.h"
#include "ParticleRenderPass.h"
#include "TransparentRenderPass.h"
#include "CanvasRenderPass.h"

//Events
#include <Events/RenderEvent.h>

//Entities
#include <EntityInterface.h>

//TODO: REMOVE
#include "Canvas.h"

RenderEventSystem* RenderEventSystem::m_instance = nullptr;

/**
*Render Event System Constructor.
*/
RenderEventSystem::RenderEventSystem()
{
	m_threadPool = ThreadPool::GetThreadPool();
	//std::cout << "RenderEventSystem intialized with address" << this << "\n";
	subcribedList.push_back(RenderEventCategory);
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
		eventQueue.pop_front();
		std::function<void()>f = std::bind(&RenderEventSystem::PrintReception, this);
		m_threadPool->submit<>(f);
	}
}

/**
* Function to Subscribe to all the event types listed in the local subscribe list.
*/
void RenderEventSystem::SubscribeToEvents()
{
	for (unsigned int i = 0; i < subcribedList.size(); i++)
	{
		EventManager::SubscribeToEvent(this, RenderEventCategory);
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
	Init();
}

/**
*Destructor for Renderer
*/
Renderer::~Renderer()
{
	for(auto pass : m_renderPasses)
	{
		fmemory::fdelete(pass);
	}

	fmemory::fdelete(can);

	RenderEventSystem::ShutDown();
}

/**
*Initialization function for Renderer
*/
void Renderer::Init()
{
	m_RES = RenderEventSystem::GetInstance();
}

/**
*Function to Create Buffers or Programs to be used in the next Draw cycle
*/
void Renderer::CreateDrawStates()
{
	//Draw in Wireframe mode - Comment out
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_PROGRAM_POINT_SIZE);

	can = fmemory::fnew<Canvas>();
	can->Setup();
}

/**
*Function to Set the relevant data in the draw states.
*/
void Renderer::SetDrawStates(boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities, glm::mat4 projection)
{
	m_projection = projection;
	for (u32 i = 0; i < entities->size(); i++)
	{
		RenderComponent* renderComp = entities->at(i)->GetComponent<RenderComponent>();
		ParticleEmitterComponent* particleComp = entities->at(i)->GetComponent<ParticleEmitterComponent>();

		if (renderComp || particleComp)
		{
			entities->at(i)->AddComponent<PhysicsComponent>();
			PhysicsComponent* physComp = entities->at(i)->GetComponent<PhysicsComponent>();

			if (renderComp)
			{
				if (i != 1)
				{
					physComp->SetBoxCollider(5, 5, 5);
					physComp->SetPhysicsBodyType(entities->at(i)->GetTransform(), physics::PhysicsBodyType::ESTATIC_BODY);

				}
				else
				{
					std::vector < glm::vec3, fmemory::STLAllocator<glm::vec3>> temp;
					renderComp->m_mesh->GetVertexPositionsArray(temp);
					//physComp->SetSphereCollider(2);//SetMeshCollider(temp, renderComp->m_mesh->m_vertexArray.size(), sizeof(glm::vec3));

					physComp->SetMeshCollider(&temp[0], temp.size(), sizeof(glm::vec3));
					physComp->SetPhysicsBodyType(entities->at(i)->GetTransform(), physics::PhysicsBodyType::EDYNAMIC_BODY);
					//delete temp;
				}
			}

			if (particleComp)
			{
				physComp->SetBoxCollider(5, 5, 5);
				physComp->SetPhysicsBodyType(entities->at(i)->GetTransform(), physics::PhysicsBodyType::ESTATIC_BODY);
			}
		}
	}

	m_renderPasses.push_back(fmemory::fnew<MeshRenderPass>(0));
	m_renderPasses.push_back(fmemory::fnew<ParticleRenderPass>(1));
	m_renderPasses.push_back(fmemory::fnew<TransparentRenderPass>(2));
	m_renderPasses.push_back(fmemory::fnew<CanvasRenderPass>(3));

	m_renderPasses[3]->QueueRenderable(can);
}

/**
 * Function that provides consistent updates for the next rendering frame.
 *
 *@param[in] An integer indicating width.
 *@param[in] An integer indicating height.
 *@param[in] A camera to use for rendering
 *@param[in] A float indicating delta time for the current frame.
 */

float temp = 0.0f;
void Renderer::Update(Camera& cam, float dt, boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities)
{
	temp += 1.0f * dt;
	m_RES->ProcessEvents();
	m_entities = entities;
	for (unsigned int i = 0; i < m_entities->size(); ++i)
	{
		if (m_entities->at(i)->GetComponent<RenderComponent>() != nullptr)
		{
			Shader* shader = m_entities->at(i)->GetComponent<RenderComponent>()->m_mesh->GetMaterial()->m_shader;
			shader->UseShader();
			shader->SetMat4("projection", m_projection);
			shader->SetMat4("view", cam.GetViewMatrix());
		}

		if(m_entities->at(i)->GetComponent<ParticleEmitterComponent>() != nullptr)
		{
			Shader* shader = m_entities->at(i)->GetComponent<ParticleEmitterComponent>()->m_particle->GetMaterial()->m_shader;
			shader->UseShader();
			shader->SetMat4("projection", m_projection);
			shader->SetMat4("view", cam.GetViewMatrix());
			shader->SetVec3("camPos", cam.m_Position);
		}
	}
	//entities->at(0)->GetTransform()->SetRotation(glm::angleAxis(temp, glm::vec3(0.0f,1.0f,0.0f)));
	m_entities->at(1)->GetTransform()->SetRotation(glm::angleAxis(temp, glm::vec3(0.0f, 0.0f, 1.0f)));
}

/**
 * Main Draw Function for the Renderer
 */
void Renderer::Draw(Camera &cam)
{
	glDepthMask(true);
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	boost::container::flat_map<float, int> distanceEntityMap;
	for (u32 i = 0; i < m_entities->size(); i++)
	{
		Transform* trans = m_entities->at(i)->GetTransform();

		RenderComponent* rc = m_entities->at(i)->GetComponent<RenderComponent>();
		if (rc)
		{
			Mesh* m = rc->m_mesh;

			if(!m->GetTransparent())
			{
				m->AddWorldMatrix(trans->GetModel());
				m_renderPasses[0]->QueueRenderable(m);
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
			m_renderPasses[1]->QueueRenderable(p);
		}
	}

	uint32_t count = 0;
	//Transparent meshes
	for(auto it = distanceEntityMap.rbegin(); it != distanceEntityMap.rend(); it++)
	{
		RenderComponent* rc = m_entities->at(it->second)->GetComponent<RenderComponent>();
		rc->m_mesh->AddWorldMatrix(m_entities->at(it->second)->GetTransform()->GetModel());
		count++;

		auto next = it;
		next++;

		if(next != distanceEntityMap.rend())
		{
			if(m_entities->at(next->second)->GetComponent<RenderComponent>()->m_mesh != rc->m_mesh)
			{
				m_renderPasses[2]->QueueRenderable(rc->m_mesh);
				static_cast<TransparentRenderPass*>(m_renderPasses[2])->AddCountAndOffset(count, rc->m_mesh->GetWorldMatrixAmount() - count);
				count = 0;
			}

			else
				continue;
		}

		else
		{
			m_renderPasses[2]->QueueRenderable(rc->m_mesh);
			static_cast<TransparentRenderPass*>(m_renderPasses[2])->AddCountAndOffset(count, rc->m_mesh->GetWorldMatrixAmount() - count);
			count = 0;
		}
	}

	for (u32 i = 0; i < m_renderPasses.size(); i++)
	{
		m_renderPasses[i]->Render();
	}
}
