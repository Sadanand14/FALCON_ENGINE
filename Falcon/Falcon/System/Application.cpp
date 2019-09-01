#include "Application.h"
Application* Application::s_Instance = nullptr;

Application::Application()
	:m_Camera(glm::vec3(0.0f, 0.0f, 5.0f))
{
	Log::Init();

	s_Instance = this;

	m_Window = new Window(1600, 900);


	m_Renderer = new Renderer(); // creates a new renderer class on the heap
	//Create Draw States in Renderer
	m_Renderer->CreateDrawStates();
	//Set Draw States in Renderer
	m_Renderer->SetDrawStates();

	m_Timer = new Timer();// creates a new timer class in the heap

	//Input

	input = new InputReceiver(m_Window);
	//Todo: May be add events class init here and connect all callbacks.

}

Application::~Application()
{
	delete m_Window;
	delete m_Renderer;
	delete m_Timer;
}

void Application::Run()
{
	float dt;
	while (!m_Window->GetWindowShouldClose())
	{
		m_Timer->update();
		dt = m_Timer->GetDeltaTime();

		//Temp Game Input should be moved out from window class.

		m_Window->ProcessInput(m_Window->GetGLFWWindow(), dt);

		//Camera
		glm::mat4 view = m_Camera.GetViewMatrix();

		//Render
		m_Renderer->Update(m_Window->GetWidth(), m_Window->GetHeight(), m_Camera.m_Zoom, view, dt);
		m_Renderer->Draw();

		m_Window->Update();

	}
}


//To be defined by client
//Application* CreateApplication();