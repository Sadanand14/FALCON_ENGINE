#include "Game.h"

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Camera Setup
float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

void ProcessInput(GLFWwindow* window, float deltaTime);

Game::Game() : m_gameCrashed(false), m_windowClosed(false)
{

}

Game::~Game()
{
	fmemory::fdelete<Timer>(m_timer);
	fmemory::fdelete<Renderer>(m_renderer);
	fmemory::fdelete<InputReceiver>(m_inputClass);
	fmemory::fdelete<WindowClass>(m_window1);
	fmemory::MeoryManagerShutDown();
}

bool Game::Initialize()
{
	Log::Init();

	fmemory::MemoryManagerInit();

	m_window1 = fmemory::fnew<WindowClass>("FalconEngine", 1280, 720);
	m_inputClass = fmemory::fnew<InputReceiver>(m_window1);
	m_renderer = fmemory::fnew<Renderer>(); // creates a new renderer class on the heap
	m_animator = fmemory::fnew<AnimationSystem>();
	m_timer = fmemory::fnew<Timer>(); // creates a new timer class in the heap

	//Camera
	glfwSetCursorPosCallback(m_window1->GetWindow(), mouse_callback);
	glfwSetScrollCallback(m_window1->GetWindow(), scroll_callback);

	//Create Draw States in Renderer
	m_renderer->CreateDrawStates();

	//Set Draw States in Renderer
	m_renderer->SetDrawStates();

	Entity* ents = m_renderer->GetEntities();

	for(u32 i = 0; i < 50; i++) {
		m_animator->RegisterComponent(ents[i].GetComponent<AnimationComponent>());
	}

	return true;
}

void Game::Update()
{
	if (!m_window1->WindowCloseStatus())
	{
		float dt, rate;
		std::string framerate;

		//testing RenderEvents
		for (unsigned int i = 0; i < 10; i++)
			EventManager::PushEvent(boost::make_shared<RenderEvent>(), RenderEventCategory);

		m_timer->update();
		dt = m_timer->GetDeltaTime();
		rate = 1 / dt;
		framerate = std::to_string(rate);
		glfwSetWindowTitle(m_window1->GetWindow(), framerate.c_str());

		//Camera
		glm::mat4 view = camera.GetViewMatrix();

		//Update animation animation system
		m_animator->Update();

		//Render
		m_renderer->Update(m_window1->GetWidth(), m_window1->GetHeight(), camera.m_Zoom, view, dt);
		m_renderer->Draw();

		//Game Input
		ProcessInput(m_window1->GetWindow(), dt);

		//Swap Buffers
		glfwSwapBuffers(m_window1->GetWindow());

		//Poll I/O events
		glfwPollEvents();

	}
	else
	{
		m_windowClosed = true;
	}
}

void ProcessInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}
