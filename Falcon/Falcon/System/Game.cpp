#include "Game.h"

namespace gameLoop
{

	//Camera
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

	//Camera Setup
	float lastX = 0.0f;
	float lastY = 0.0f;
	bool firstMouse = true;

	void ProcessInput(GLFWwindow* window, float deltaTime);

	Game::Game() : m_gameCrashed(false), m_windowClosed(false)
	{
		//m_scene = new Scene();
	}

	Game::~Game()
	{
		//m_scene->SaveScene("../Assets/Scenes/scene2.json");
		//delete m_scene;
		fmemory::fdelete<Scene::SceneGraph>(m_scene);
		fmemory::fdelete<Scene::Octree>(m_octree);
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
		m_timer = fmemory::fnew<Timer>(); // creates a new timer class in the heap
		m_scene = fmemory::fnew<Scene::SceneGraph>("../Assets/Scenes/scene.json");
		m_scene->UpdateScene();

		m_octree = fmemory::fnew<Scene::Octree>(glm::vec3(-320.0f, 320.0f, -320.0f), glm::vec3(320.0f, -320.0f, 320.0f), 10.0f, m_scene, &camera);

		m_renderer->SetEntities(m_scene->GetEntities());

		//Camera
		glfwSetCursorPosCallback(m_window1->GetWindow(), mouse_callback);
		glfwSetScrollCallback(m_window1->GetWindow(), scroll_callback);

		//Create Draw States in Renderer
		m_renderer->CreateDrawStates();

		//calculate Projection temporarily here
		glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), (float)m_window1->GetWidth() / (float)m_window1->GetHeight(), 0.1f, 100.0f);

		//m_octree->SetProjection(projection);
		//Set Draw States in Renderer
		m_renderer->SetDrawStates( projection);

		//m_scene->LoadScene("../Assets/Scenes/scene.json");
		//m_octree->Distribute();



		return true;
	}

	void Game::Update()
	{
		while (!m_window1->WindowCloseStatus())
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

			//renderer Update
			m_renderer->Update(camera.GetViewMatrix(),dt);

			//Update SceneGraph
			m_scene->UpdateScene();

			m_octree->Update();
			//Render
			m_renderer->Draw();


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
