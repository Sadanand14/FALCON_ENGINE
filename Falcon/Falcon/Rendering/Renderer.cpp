#include "Renderer.h"
#include "Memory/fmemory.h"

Renderer::Renderer()
{
	Init();
}

Renderer::~Renderer()
{
	fmemory::fdelete<Shader>(m_shadyStuff);
	fmemory::fdelete<Model>(m_nanosuit);
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

	//Load Model
	m_nanosuit = fmemory::fnew<Model>("../Assets/Models/nanosuit/nanosuit.obj");	

	//Create Shader Program and bind a Vertex shader and Fragment shader.
	m_shadyStuff = fmemory::fnew<Shader>("Shader/VertexShader.vert", "Shader/FragmentShader.frag");
}

void Renderer::SetDrawStates()
{

	//Use the Program Shader
	m_shadyStuff->UseShader();
}

void Renderer::Update(int width, int height, float zoom, glm::mat4 view, float dt)
{
	glm::mat4 projection = glm::perspective(glm::radians(zoom), (float)width / (float)height, 0.1f, 100.0f);
	m_shadyStuff->SetMat4("projection", projection);

	// camera/view transformations
	m_shadyStuff->SetMat4("view", view);

	// Model transformations
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	m_shadyStuff->SetMat4("model", model);
}

void Renderer::Draw()
{
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_nanosuit->Draw(*m_shadyStuff);
}
