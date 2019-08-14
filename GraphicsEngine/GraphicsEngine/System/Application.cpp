#include "Application.h"
#include "Log.h"
#include "glm/vec3.hpp"
#include "framework.h"
#include "System/Input/Input.h"
#include "events/ApplicationEvent.h"

Application* Application::s_Instance = nullptr;

Application::Application():
	m_Camera(glm::vec3(0.0f, 0.0f, 3.0f))
{	
	Log::Init();

	s_Instance = this;

	m_Window = new Window(1600, 900);
	m_Window->SetWindowsEventCallbackFunction(BIND_EVENT_FN(Application::OnEvent));


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
	events::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<events::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent));
	dispatcher.Dispatch<events::KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPress));
	dispatcher.Dispatch<events::MouseMovedEvent>(BIND_EVENT_FN(Application::OnMouseMove));
	dispatcher.Dispatch<events::MouseScrolledEvent>(BIND_EVENT_FN(Application::OnMouseScroll));
	//FL_ENGINE_INFO("Event catched {0}", e.ToString());
}

bool Application::OnWindowCloseEvent(events::WindowCloseEvent& e)
{
	FL_ENGINE_INFO("Closing the window");
	m_Window->SetWindowShouldClose(true);
	return true;
}

bool Application::OnKeyPress(events::KeyPressedEvent& e)
{
	if (Input::GetKeyDown(FL_KEY_ESCAPE))
	{
		m_Window->SetWindowShouldClose(true);
	}
	else if (Input::GetKeyDown(FL_KEY_W))
	{
		m_Camera.Move(Camera_Movement::FORWARD, m_Timer->GetDeltaTime());
	}
	else if (Input::GetKeyDown(FL_KEY_S))
	{
		m_Camera.Move(Camera_Movement::BACKWARD, m_Timer->GetDeltaTime());
	}
	else if (Input::GetKeyDown(FL_KEY_A))
	{
		m_Camera.Move(Camera_Movement::LEFT, m_Timer->GetDeltaTime());
	}
	else if (Input::GetKeyDown(FL_KEY_D))
	{
		m_Camera.Move(Camera_Movement::RIGHT, m_Timer->GetDeltaTime());
	}
	return true;
}

bool Application::OnMouseMove(events::MouseMovedEvent& e)
{
	float xpos = e.GetX();
	float ypos = e.GetY();

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

	m_Camera.Rotate(xoffset, yoffset);
	return true;
}

bool Application::OnMouseScroll(events::MouseScrolledEvent& e)
{
	float y = e.GetYOffset();
	m_Camera.Zoom(y);
	return true;

}

void Application::Run()
{
	while (!m_Window->GetWindowShouldClose())
	{
		
		
		//Adding renderer logic here. 
		m_Timer->update();
		float dt = m_Timer->GetDeltaTime();

		if (Input::GetKeyDown(FL_KEY_SPACE))
		{
			FL_ENGINE_TRACE("ouch!!");
		}

		//Camera
		glm::mat4 view = m_Camera.GetViewMatrix();

		//Render
		m_Renderer->Update(m_Window->GetWidth(),m_Window->GetHeight(), m_Camera.GetZoom(), view, dt);
		m_Renderer->Draw();

		
		m_Window->Update();
	}
}



