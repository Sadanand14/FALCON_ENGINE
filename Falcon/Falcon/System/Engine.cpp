#include "Engine.h"
#include "Physics/Physics.h"

namespace gameLoop
{

	//Camera
	Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

	/**
	* Game class Destructor
	*/
	Engine::~Engine()
	{
		if (m_scene != nullptr)fmemory::fdelete(m_scene);
		if (m_octree != nullptr)fmemory::fdelete(m_octree);
		fmemory::fdelete(m_timer);
		fmemory::fdelete(m_particleSystem);
		fmemory::fdelete(m_renderer);
		fmemory::fdelete(m_input);
		fmemory::fdelete(m_window);
		AssetManager::Clean();
		physics::ShutdownPhysX();
		fmemory::MeoryManagerShutDown();

		m_audio.UnLoadSound("../Assets/Sounds/f1_theme_brian_tyler.wav");
		m_audio.Shutdown();
		ThreadPool::ShutDown();
	}

	/**
	* Game Class Constructor
	*/
	Engine::Engine() : m_gameCrashed(false), m_windowClosed(false)
	{

	}

	/**
	* Game class initializer responsible for initializing all subsystems.
	*/
	bool Engine::Initialize()
	{
		Log::Init();

		m_currentState = STATE::MENU;

		fmemory::MemoryManagerInit();

		m_window = fmemory::fnew<WindowClass>("FalconEngine", 1280, 720);
		m_threads = ThreadPool::GetThreadPool(m_window->GetWindow());

		m_input = fmemory::fnew<InputReceiver>(m_window);
		m_input->Init(m_window->GetWindow());

		//Booting up physics
		physics::InitPhysX();

		m_renderer = fmemory::fnew<Renderer>(); // creates a new renderer class on the heap
		m_renderer->Init(m_window->GetWindow());

		m_timer = fmemory::fnew<Timer>(); // creates a new timer class in the heap
		m_scene = fmemory::fnew<Scene::SceneGraph>("../Assets/Scenes/scene.json");
		m_particleSystem = fmemory::fnew<ParticleSystem>();
		m_scene->UpdateScene();

		m_octree = fmemory::fnew<Rendering::Octree>(glm::vec3(-320.0f, 320.0f, -320.0f), glm::vec3(320.0f, -320.0f, 320.0f), 5.0f, m_scene, &camera);
		//calculate Projection temporarily here
		glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), (float)m_window->GetWidth() / (float)m_window->GetHeight(), 0.1f, 100.0f);
		m_octree->SetProjection(projection);
		m_octree->Update();

		//Create Draw States in Renderer
		m_renderer->CreateDrawStates(m_window->GetWindow());

		//Set Draw States in Renderer
		m_renderer->SetDrawStates(m_octree->GetViewables(), projection);

		//Initialize the Audio Engine
		m_audio.Init();

		m_audio.LoadSound("../Assets/Sounds/f1_theme_brian_tyler.wav", true, true, false);
		//m_audio.PlaySounds("../Assets/Sounds/f1_theme_brian_tyler.wav", { 0,0,0 }, -0.6f);

		return true;
	}

	void Engine::MenuUpdate()
	{
		m_renderer->Menu_Update();
		m_renderer->Menu_Draw();
		m_input->Update();
	}

	void Engine::IngameUpdate()
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
		glfwSetWindowTitle(m_window->GetWindow(), framerate.c_str());

		//Update SceneGraph
		m_scene->UpdateScene();

		m_octree->Update();

		m_particleSystem->Update(dt, m_scene->GetEntities());
		////renderer Update
		m_renderer->Ingame_Update(dt, m_scene->GetEntities());
		m_renderer->Ingame_Draw();

		physics::StepPhysics(dt, m_scene->GetEntities(), m_scene->GetEntities()->size());


		camera.SetMousePos(m_input->GetCursor());
		camera.SetMouseScroll(m_input->GetScroll());

		//camera movement setup
		ProcessInputs(dt);

		//static unsigned int temp = Rendering::OctreeNode::GetCount();
		//FL_ENGINE_WARN("NodeCount: {0}", temp);

		m_input->Update();
	}

	void Engine::PauseUpdate()
	{
		m_input->Update();
	}

	void Engine::ProcessInputs(float dt)
	{
		/*if (m_input->GetKeyPress(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(m_window->GetWindow(), true);
		if (m_input->GetKey(GLFW_KEY_W))camera.ProcessKeyboard(FORWARD, dt);
		if (m_input->GetKey(GLFW_KEY_A))camera.ProcessKeyboard(LEFT, dt);
		if (m_input->GetKey(GLFW_KEY_D))camera.ProcessKeyboard(RIGHT, dt);
		if (m_input->GetKey(GLFW_KEY_S))camera.ProcessKeyboard(BACKWARD, dt);*/
	}
}
