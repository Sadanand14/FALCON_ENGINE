#include "Game.h"
#include "Physics/Physics.h"
#include <EntityManager.h>
#include <ThreadPool.h>
namespace gameLoop
{
	//Camera
	Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

	//Camera Setup
	float lastX = 0.0f;
	float lastY = 0.0f;
	bool firstMouse = true;
	void ProcessInput(GLFWwindow* window, float deltaTime);

	Game::~Game()
	{
		fmemory::fdelete(m_scene);
		fmemory::fdelete(m_octree);
		fmemory::fdelete(m_timer);
		fmemory::fdelete(m_particleSystem);
		fmemory::fdelete(m_renderer);
		fmemory::fdelete(m_inputClass);
		fmemory::fdelete(m_window1);
		AssetManager::Clean();
		fmemory::MeoryManagerShutDown();
		EntityManager::ClearManager();
		physics::ShutdownPhysX();
		ThreadPool::ShutDown();
	}

	Game::Game() : m_gameCrashed(false), m_windowClosed(false)
	{
	}

	bool Game::Initialize()
	{
		Log::Init();

		fmemory::MemoryManagerInit();

		m_window1 = fmemory::fnew<WindowClass>("FalconEngine", 1280, 720);
		m_inputClass = fmemory::fnew<InputReceiver>(m_window1);
		m_renderer = fmemory::fnew<Renderer>(); // creates a new renderer class on the heap
		m_timer = fmemory::fnew<Timer>(); // creates a new timer class in the heap
		m_scene = fmemory::fnew<Scene::SceneGraph>("../Assets/Scenes/scene.json");
		m_particleSystem = fmemory::fnew<ParticleSystem>();
		m_scene->UpdateScene();

		m_octree = fmemory::fnew<Rendering::Octree>(glm::vec3(-320.0f, 320.0f, -320.0f), glm::vec3(320.0f, -320.0f, 320.0f), 5.0f, m_scene, &camera);
		//calculate Projection temporarily here
		glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), (float)m_window1->GetWidth() / (float)m_window1->GetHeight(), 0.1f, 100.0f);
		m_octree->SetProjection(projection);
		m_octree->Update();

		////Booting up physics
		physics::InitPhysX();


		glfwSetCursorPosCallback(m_window1->GetWindow(), mouse_callback);
		glfwSetScrollCallback(m_window1->GetWindow(), scroll_callback);

		////Create Draw States in Renderer
		m_renderer->CreateDrawStates();
		////Set Draw States in Renderer
		m_renderer->SetDrawStates(m_octree->GetViewables());


		return true;
	}

	void Game::Update()
	{
		while (!m_window1->WindowCloseStatus())
		{
			float dt, rate;
			std::string framerate;

			//testing RenderEvents
			//for (unsigned int i = 0; i < 10; i++)
			//	EventManager::PushEvent(boost::make_shared<RenderEvent>(), RenderEventCategory);

			m_timer->update();
			dt = m_timer->GetDeltaTime();
			rate = 1 / dt;
			framerate = std::to_string(rate);
			glfwSetWindowTitle(m_window1->GetWindow(), framerate.c_str());

			//Update SceneGraph
			m_scene->UpdateScene();

			m_octree->Update();

			m_particleSystem->Update(dt, m_octree->GetViewables());

			////renderer Update
			m_renderer->Update(m_window1->GetWidth(), m_window1->GetHeight(), camera, dt, m_octree->GetViewables());
			m_renderer->Draw(m_octree->GetViewables());

			physics::StepPhysics(dt, m_scene->GetEntities(), m_scene->GetEntities()->size());

			//Poll I/O events
			glfwPollEvents();

			//Game Input
			ProcessInput(m_window1->GetWindow(), dt);

			//Swap Buffers
			glfwSwapBuffers(m_window1->GetWindow());

			//Poll I/O events
			glfwPollEvents();
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
}
