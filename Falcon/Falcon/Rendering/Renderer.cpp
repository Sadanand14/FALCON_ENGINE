#include "Renderer.h"

RenderEventSystem* RenderEventSystem::m_instance = nullptr;



RenderEventSystem::RenderEventSystem()
{
	m_threadPool = ThreadPool::GetThreadPool();
	std::cout << "RenderEventSystem intialized with address" << this << "\n";
	subcribedList.push_back(RenderEventCategory);
	SubscribeToEvents();
}

void RenderEventSystem::ProcessEvents() 
{
	FL_ENGINE_WARN("eventQueue Size: {0}", eventQueue.size());
	for (unsigned int i = 0; i < eventQueue.size(); i++)
	{
		m_threadPool->submit<void()>(PrintReception);
	}
}

void RenderEventSystem::SubscribeToEvents()
{
	for (unsigned int i = 0; i < subcribedList.size(); i++)
	{
		EventManager::SubscribeToEvent(this, RenderEventCategory);
	}
}
void PrintReception()
{
	FL_ENGINE_INFO("Event Executed SuccessFully");
	std::cout << std::this_thread::get_id() << "\n";
}
//////////////////////
Renderer::Renderer()
{
	Init();
}


Renderer::~Renderer()
{

}

//renderer initialization
void Renderer::Init()
{
	m_RES = RenderEventSystem::GetInstance();
}

void Renderer::CreateDrawStates()
{
	// Configure global opengl state
	glEnable(GL_DEPTH_TEST);

	//Draw in Wireframe mode - Comment out
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::SetDrawStates()
{
	entity = new Entity();
	entity->AddComponent<RenderComponent>();
	RenderComponent* rd = entity->GetComponent<RenderComponent>();
	rd->m_mesh = AssetManager::LoadModel("../Assets/Models/cerb/cerberus.fbx");
	//rd->m_mesh = AssetManager::LoadModel("../Assets/Models/nanosuit/nanosuit.obj");
	rd->m_material = AssetManager::LoadMaterial("../Assets/Materials/");

	rd->m_material->shader = new Shader("Shader/VertexShader.vert", "Shader/FragmentShader.frag");
	shader = rd->m_material->shader;
	shader->UseShader();
	entity->GetComponent<RenderComponent>()->m_mesh->DrawMesh();
}

void Renderer::Update(int width, int height, float zoom, glm::mat4 view, float dt)
{
	m_RES->ProcessEvents();
	glm::mat4 projection = glm::perspective(glm::radians(zoom), (float)width / (float)height, 0.1f, 100.0f);
	shader->SetMat4("projection", projection);

	// camera/view transformations
	shader->SetMat4("view", view);

	// Model transformations
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader->SetMat4("model", model);
}

void Renderer::Draw()
{
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mesh* m = entity->GetComponent<RenderComponent>()->m_mesh;
	Material* mat = entity->GetComponent<RenderComponent>()->m_material;

	if(mat != nullptr)
		mat->Bind();
	for(i32 i = 0; i < m->m_indexOffsets.size(); i++)
	{
		i32 count;
		if(i < m->m_indexOffsets.size() - 1)
			count = m->m_indexOffsets[i + 1] - m->m_indexOffsets[i];
		else
			count = m->m_indexArray.size() - m->m_indexOffsets[i];
		glDrawElementsBaseVertex(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0, m->m_indexOffsets[i]);
	}
}
