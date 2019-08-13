#include "Application.h"
#include "Log.h"
#include "glm/vec3.hpp"
#include "framework.h"
Application* Application::s_Instance = nullptr;

Application::Application():
	m_Camera(glm::vec3(0.0f, 0.0f, 3.0f))
{	
	Log::Init();

	s_Instance = this;

	m_Window = new Window(1600, 900);
	m_Window->SetWindowsCallback(BIND_EVENT_FN(Application::OnEvent));


	m_Renderer = new Renderer(); // creates a new renderer class on the heap
	//Create Draw States in Renderer
	m_Renderer->CreateDrawStates();
	//Set Draw States in Renderer
	m_Renderer->SetDrawStates();

	m_Timer = new Timer();// creates a new timer class in the heap

	//InputReceiver* input = new InputReceiver(mainWindow);
		
}

Application::~Application()
{
	delete m_Window;
}

void Application::OnEvent(events::Event& e)
{
	FL_ENGINE_INFO("Event catched {0}", e);
}

void Application::Run()
{
	while (!m_Window->GetWindowShouldClose())
	{
		
		
		//Adding renderer logic here. 
		m_Timer->update();
		float dt = m_Timer->GetDeltaTime();

		//Game Input
		m_Window->ProcessInput(m_Window->GetGLFWWindow(),m_Camera, dt);

		//Camera
		glm::mat4 view = m_Camera.GetViewMatrix();

		//Render
		m_Renderer->Update(m_Window->GetWidth(),m_Window->GetHeight(), m_Camera.m_Zoom, view, dt);
		m_Renderer->Draw();

		
		m_Window->Update();
	}
}



