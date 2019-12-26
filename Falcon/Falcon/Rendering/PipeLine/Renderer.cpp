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
	// Configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	//Draw in Wireframe mode - Comment out
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/**
*Function to Set the relevant data in the draw states.
*/
void Renderer::SetDrawStates(boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities)
{
	//entity = fmemory::fnew_arr<Entity>(500);

	//Mesh* mesh = AssetManager::LoadModel("../Assets/Models/cerb/cerberus.fbx");
	shader = fmemory::fnew<Shader>("Rendering/Shader/VertexShader.vert", "Rendering/Shader/FragmentShader.frag");
	particleShader = fmemory::fnew<Shader>("Rendering/Shader/Particle.vert", "Rendering/Shader/Particle.frag");

	for (unsigned int i = 0; i < entities->size(); i++)
	{
		if(entities->at(i)->GetComponent<RenderComponent>())
			entities->at(i)->GetComponent<RenderComponent>()->m_mesh->GetMaterial()->SetShader(shader);

		if(entities->at(i)->GetComponent<ParticleEmitterComponent>()) {
			entities->at(i)->GetComponent<ParticleEmitterComponent>()->m_particle->GetMaterial()->SetShader(particleShader);
		}
	}
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
void Renderer::Update(int width, int height, Camera &cam, float dt, boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities)
{
	temp += 1.0f * dt;
	m_RES->ProcessEvents();
	glm::mat4 projection = glm::perspective(glm::radians(cam.m_Zoom), (float)width / (float)height, 0.1f, 100.0f);

	shader->UseShader();
	shader->SetMat4("projection", projection);
	shader->SetMat4("view", cam.GetViewMatrix());

	particleShader->UseShader();
	particleShader->SetMat4("projection", projection);
	particleShader->SetMat4("view", cam.GetViewMatrix());
	particleShader->SetVec3("camPos", cam.m_Position);

	//entities->at(0)->GetTransform()->SetRotation(glm::angleAxis(temp, glm::vec3(0.0f,1.0f,0.0f)));
	entities->at(1)->GetTransform()->SetRotation(glm::angleAxis(temp, glm::vec3(0.0f,0.0f,1.0f)));

}

/**
* Main Draw Function for the Renderer
*/
//TODO-> Have multiple Renderer Passes functionality
void Renderer::Draw(boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entities)
{
	for (u32 i = 0; i < entities->size(); i++) {
		Transform* trans = entities->at(i)->GetTransform();
		RenderComponent* renderComp = entities->at(i)->GetComponent<RenderComponent>();

		if(renderComp)
		{
			Mesh* m = renderComp->m_mesh;
			m->AddWorldMatrix(trans->GetModel());

			if (queuedMeshes.find(m) == queuedMeshes.end())
				queuedMeshes.insert(m);
		}

		ParticleEmitterComponent* particleComp = entities->at(i)->GetComponent<ParticleEmitterComponent>();

		if(particleComp)
		{
			particleComp->m_particle->SetWorldMatrix(trans->GetModel());
			for(auto it = particleComp->m_particleBuffer.begin(); it != particleComp->m_particleBuffer.end(); it++)
				particleComp->m_particle->AddParticleData(*it);

			queuedParticles.push_back(particleComp->m_particle);
		}
	}

	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(auto it = queuedMeshes.begin(); it != queuedMeshes.end(); it++)
	{
		Shader* shad = (*it)->GetMaterial()->m_shader;
		shad->UseShader();
		(*it)->Bind();

		for(u32 i = 0; i < (*it)->m_indexOffsets.size(); i++)
		{
			i32 count;
			if (i < (*it)->m_indexOffsets.size() - 1)
				count = (*it)->m_indexOffsets[i + 1] - (*it)->m_indexOffsets[i];
			else
				count = (*it)->m_indexArray.size() - (*it)->m_indexOffsets[i];
			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0, (*it)->GetWorldMatrixAmount(), (*it)->m_indexOffsets[i]);
		}
		(*it)->ClearWorldMatrices();
	}

	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(false);

	for(u32 i = 0; i < queuedParticles.size(); i++)
	{
		Shader* shad = queuedParticles[i]->GetMaterial()->m_shader;
		shad->UseShader();
		shad->SetMat4("model", queuedParticles[i]->GetWorldMatrix());
		queuedParticles[i]->Bind();
		glDrawArraysInstanced(GL_POINTS, 0, 1, queuedParticles[i]->GetParticleDataAmount());
		queuedParticles[i]->ClearParticleData();
	}
	glDepthMask(true);

	queuedMeshes.clear();
	queuedParticles.clear();
}
