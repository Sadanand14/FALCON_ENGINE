#include "Renderer.h"

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
	delete m_shadyStuff;
	delete m_nanosuit;
}

/**
*Initialization function for Renderer
*/
void Renderer::Init()
{	
	
}

/**
*Function to Create Buffers or Programs to be used in the next Draw cycle
*/
void Renderer::CreateDrawStates()
{

	// Configure global opengl state
	glEnable(GL_DEPTH_TEST);

	//Draw in Wireframe mode - Comment out
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	

	//Load Model
	m_nanosuit = new Model("../Assets/Models/nanosuit/nanosuit.obj");	

	//Create Shader Program and bind a Vertex shader and Fragment shader.
	m_shadyStuff = new Shader("Shader/VertexShader.vert", "Shader/FragmentShader.frag");
}

/**
*Function to Set the relevant data in the draw states.
*/
void Renderer::SetDrawStates()
{

	//Use the Program Shader
	m_shadyStuff->UseShader();
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

/**
* Main Draw Function for the Renderer
*/
//TODO-> Have multiple Renderer Passes functionality
void Renderer::Draw()
{
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_nanosuit->Draw(*m_shadyStuff);
}
