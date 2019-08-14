#ifndef APPLICATION_H
#define APPLICATION_H

#include "framework.h"
#include "Rendering/WindowHandler.h"
#include "Rendering/Renderer.h"
#include "Rendering/Camera.h"
#include "events/Event.h"
#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

class Application
{

private:
	static Application* s_Instance;
	Window* m_Window;
	Renderer* m_Renderer;
	Timer* m_Timer;
	//Camera
	Camera m_Camera;
	//Camera Setup	
	float lastX = 0.0f;
	float lastY = 0.0f;
	bool firstMouse = true;
	
public:
	static inline Application& GetInstance() { return *s_Instance; }
	Application();
	//Game classes are supposed to extend the application and define destructor
	virtual ~Application();
	void Run();
	void OnEvent(events::Event &e);
	
	inline const Window& GetWindow() const { return *m_Window; }
	
	
	//Event handling methods
	bool OnWindowCloseEvent(events::WindowCloseEvent& e);

	//Input handling methods. These currently handle Camera, but should be moved to other layer later on.
	bool OnKeyPress(events::KeyPressedEvent& e);
	bool OnMouseMove(events::MouseMovedEvent& e);
	bool OnMouseScroll(events::MouseScrolledEvent& e);
};

//To be defined by client
//Application* CreateApplication();



#endif // !APPLICATION_H