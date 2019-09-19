#include "Renderer.h"


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

}

void Renderer::CreateDrawStates()
{

	// Configure global opengl state
	glEnable(GL_DEPTH_TEST);

	//Draw in Wireframe mode - Comment out
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}

void Renderer::SetDrawStates()
{
	entity = new Entity();
	entity->AddComponent<RenderComponent>();
	RenderComponent* rd = entity->GetComponent<RenderComponent>();
	rd->m_mesh = AssetManager::LoadModel("../Assets/Models/nanosuit/nanosuit.obj");

	shader = new Shader("Shader/VertexShader.vert", "Shader/FragmentShader.frag");
	entity->GetComponent<RenderComponent>()->m_mesh->DrawMesh(*shader);
}

void Renderer::Update(int width, int height, float zoom, glm::mat4 view, float dt)
{
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
	glDrawElements(GL_TRIANGLES, (GLsizei)entity->GetComponent<RenderComponent>()->m_mesh->m_indexArray.size(), GL_UNSIGNED_INT,(const GLvoid*)entity->GetComponent<RenderComponent>()->m_mesh->m_indexArray.data());
}
