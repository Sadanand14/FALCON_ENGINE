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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//Draw in Wireframe mode - Comment out
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/**
*Function to Set the relevant data in the draw states.
*/
void Renderer::SetDrawStates( glm::mat4 projection)
{
	//entity = fmemory::fnew_arr<Entity>(500);

	//Mesh* mesh = AssetManager::LoadModel("../Assets/Models/cerb/cerberus.fbx");
	shader = fmemory::fnew<Shader>("Rendering/Shader/VertexShader.vert", "Rendering/Shader/FragmentShader.frag");
	
	shader->UseShader();
	shader->SetMat4("projection", projection);
	
	

	//for (u32 i = 0; i < 500; i++) {
	//	entity[i].AddComponent<RenderComponent>();
	//	RenderComponent* rd = entity[i].GetComponent<RenderComponent>();
	//	rd->m_mesh = mesh;//AssetManager::LoadModel("../Assets/Models/cerb/cerberus.fbx");
	//	//rd->m_mesh = AssetManager::LoadModel("../Assets/Models/nanosuit/nanosuit.obj");
	//	//rd->GetMaterial()->m_shader = shader;

	//	glm::vec3 pos = glm::vec3(float(std::rand() % 100 - 50), float(std::rand() % 100 - 50), float(std::rand() % 100 - 50));
	//	// Model transformations
	//	entity[i].GetTransform()->SetPosition(pos);
	//	entity[i].GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	//}
	for (unsigned int i = 0; i < m_entity.size(); i++) 
	{
		m_entity[i]->GetComponent<RenderComponent>()->m_mesh->GetMaterial()->SetShader(shader);
		m_entity[i]->AddComponent<PhysicsComponent>();
		if (i == 0)
		{
			m_entity[i]->GetComponent<PhysicsComponent>()->SetBoxCollider(5, 5, 5);
			m_entity[i]->GetComponent<PhysicsComponent>()->SetPhysicsBodyType(m_entity[i]->GetTransform(),physics::PhysicsBodyType::ESTATIC_BODY);

		}
		else
		{
			RenderComponent* renderComp = m_entity[i]->GetComponent<RenderComponent>();
			glm::vec3* temp = renderComp->m_mesh->GetVertexPositionsArray();
			m_entity[i]->GetComponent<PhysicsComponent>()->SetSphereCollider(2);//SetMeshCollider(temp, renderComp->m_mesh->m_vertexArray.size(), sizeof(glm::vec3));
			m_entity[i]->GetComponent<PhysicsComponent>()->SetPhysicsBodyType(m_entity[i]->GetTransform(), physics::PhysicsBodyType::EDYNAMIC_BODY);
			//delete temp;
		}
	}
}

/**
* Function that provides consistent updates for the next rendering frame.
*
*@param[in] An integer indicating width.
*@param[in] An integer indicating height.
*@param[in] A float indicating zoom.
*@param[in] A 4x4 matrix defined in glm library.
*@param[in] A float indicating delta time for the current frame.
*/

float tempX = 0.0f;
void Renderer::Update(glm::mat4 view,float dt)
{ 
	static float multiplier = 10.0f;
	//FL_ENGINE_INFO("Temp VAlue : {0}, multiplier: {1}", tempX, multiplier);
	FL_ENGINE_INFO("Draw Count: {0}", m_entity.size());
	for (unsigned int i = 0; i < m_entity.size(); i++)
	{
		m_entity[i]->GetComponent<RenderComponent>()->m_mesh->GetMaterial()->SetShader(shader);
	}
	if (tempX > 100.0f)
	{
		multiplier *= -1.0f; tempX = 10.0f - FLT_MIN;
	}
	if (tempX < -100.0f) 
	{ multiplier *= -1.0f; tempX = -10.0f + FLT_MIN; }

		
	tempX += dt * multiplier;
	m_RES->ProcessEvents();

	//
	// camera/view transformations
	shader->SetMat4("view", view); 
	//m_entity[0]->GetTransform()->SetPosition(glm::vec3(tempX, 10.0f,-10.0f));
	/*m_entity[0]->GetTransform()->SetRotation(glm::angleAxis(temp, glm::vec3(0.0f,1.0f,0.0f)));
	for (unsigned int i = 1; i < m_entity.size(); i++) 
	{
		m_entity[i]->GetTransform()->SetRotation(glm::angleAxis(temp, glm::vec3(0.0f,0.0f,1.0f)));
	}*/

}

/**
* Main Draw Function for the Renderer
*/
//TODO-> Have multiple Renderer Passes functionality
void Renderer::Draw()
{
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (u32 i = 0; i < m_entity.size(); i++) {
		Mesh* m = m_entity[i]->GetComponent<RenderComponent>()->m_mesh;

		m->AddWorldMatrix(m_entity[i]->GetTransform()->GetModel());

		if (queuedMeshes.find(m) == queuedMeshes.end())
			queuedMeshes.insert(m);
	}

	for (auto it = queuedMeshes.begin(); it != queuedMeshes.end(); it++) {
		(*it)->Bind();

		for (u32 i = 0; i < (*it)->m_indexOffsets.size(); i++)
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

	queuedMeshes.clear();
}