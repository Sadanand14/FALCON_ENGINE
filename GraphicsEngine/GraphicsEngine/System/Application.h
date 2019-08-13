#ifndef APPLICATION_H
#define APPLICATION_H

#include "framework.h"
#include "Rendering/WindowHandler.h"
#include "Rendering/Renderer.h"
#include "events/Event.h"

class Application
{

private:
	static Application* s_Instance;
	Window* m_Window;
	Renderer* m_Renderer;
	Timer* m_Timer;
	//Camera
	Camera m_Camera;

public:
	static inline Application& GetInstance() { return *s_Instance; }
	Application();
	//Game classes are supposed to extend the application and define destructor
	virtual ~Application();
	void Run();
	void OnEvent(events::Event &e);
	
	inline const Window& GetWindow() const { return *m_Window; }
	
};

//To be defined by client
Application* CreateApplication();



#endif // !APPLICATION_H