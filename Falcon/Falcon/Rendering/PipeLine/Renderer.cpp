#include "Renderer.h"
#include <Memory/fmemory.h>

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
}

/**
*Function to Set the relevant data in the draw states.
*/
void Renderer::SetDrawStates(boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities, glm::mat4 projection)
{
	//entity = fmemory::fnew_arr<Entity>(500);

	//Mesh* mesh = AssetManager::LoadModel("../Assets/Models/cerb/cerberus.fbx");
	//shader = fmemory::fnew<Shader>("Rendering/Shader/VertexShader.vert", "Rendering/Shader/FragmentShader.frag");
	particleShader = fmemory::fnew<Shader>("Rendering/Shader/Particle.vert", "Rendering/Shader/Particle.frag");

	m_projection = projection;
	for (u32 i = 0; i < entities->size(); i++)
	{
		RenderComponent* renderComp = entities->at(i)->GetComponent<RenderComponent>();
		ParticleEmitterComponent* particleComp = entities->at(i)->GetComponent<ParticleEmitterComponent>();

		if(renderComp || particleComp)
		{
			entities->at(i)->AddComponent<PhysicsComponent>();
			PhysicsComponent* physComp = entities->at(i)->GetComponent<PhysicsComponent>();

			if(renderComp)
			{
				//renderComp->m_mesh->GetMaterial()->SetShader(shader);

				if (i == 0)
				{
					physComp->SetBoxCollider(5, 5, 5);
					physComp->SetPhysicsBodyType(entities->at(i)->GetTransform(), physics::PhysicsBodyType::ESTATIC_BODY);

				}
				else
				{
					//glm::vec3* temp = renderComp->m_mesh->GetVertexPositionsArray();
					physComp->SetSphereCollider(2);//SetMeshCollider(temp, renderComp->m_mesh->m_vertexArray.size(), sizeof(glm::vec3));
					physComp->SetPhysicsBodyType(entities->at(i)->GetTransform(), physics::PhysicsBodyType::EDYNAMIC_BODY);
					//delete temp;
				}
			}

			if(particleComp)
			{
				particleComp->m_particle->GetMaterial()->SetShader(particleShader);
				physComp->SetBoxCollider(5, 5, 5);
				physComp->SetPhysicsBodyType(entities->at(i)->GetTransform(), physics::PhysicsBodyType::ESTATIC_BODY);
			}
		}
	}

	m_renderPasses.push_back(fmemory::fnew<MeshRenderPass>(0));
	m_renderPasses.push_back(fmemory::fnew<ParticleRenderPass>(1));
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
void Renderer::Update(Camera &cam, float dt, boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities)
{
	temp += 1.0f * dt;
	m_RES->ProcessEvents();
	m_entities = entities;
	for (unsigned int i = 0; i < m_entities->size(); ++i)
	{
		Shader* shader = m_entities->at(i)->GetComponent<RenderComponent>()->m_mesh->GetMaterial()->m_shader;
		shader->UseShader();
		shader->SetMat4("projection", m_projection);
		shader->SetMat4("view", cam.GetViewMatrix());
	}

	particleShader->UseShader();
	
	particleShader->SetMat4("projection", m_projection);
	particleShader->SetMat4("view", cam.GetViewMatrix());
	particleShader->SetVec3("camPos", cam.m_Position);

	//entities->at(0)->GetTransform()->SetRotation(glm::angleAxis(temp, glm::vec3(0.0f,1.0f,0.0f)));
	m_entities->at(1)->GetTransform()->SetRotation(glm::angleAxis(temp, glm::vec3(0.0f,0.0f,1.0f)));
}

/**
* Main Draw Function for the Renderer
*/
//TODO-> Have multiple Renderer Passes functionality
void Renderer::Draw()
{
	glDepthMask(true);
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (u32 i = 0; i < m_entities->size(); i++)
	{
		Transform* trans = m_entities->at(i)->GetTransform();

		RenderComponent* rc = m_entities->at(i)->GetComponent<RenderComponent>();
		if(rc)
		{
			Mesh* m = rc->m_mesh;
			m->AddWorldMatrix(trans->GetModel());
			m_renderPasses[0]->QueueRenderable(m);
		}

		ParticleEmitterComponent* pec = m_entities->at(i)->GetComponent<ParticleEmitterComponent>();
		if(pec)
		{
			Particle* p = pec->m_particle;
			p->SetWorldMatrix(trans->GetModel());
			for(auto it = pec->m_particleBuffer.begin(); it != pec->m_particleBuffer.end(); it++)
				p->AddParticleData(*it);
			m_renderPasses[1]->QueueRenderable(p);
		}
	}

	for (u32 i = 0; i < m_renderPasses.size(); i++)
	{
		m_renderPasses[i]->Render();
	}
}
