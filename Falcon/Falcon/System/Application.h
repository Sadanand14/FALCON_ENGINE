#ifndef APPLICATION_H
#define APPLICATION_H

#include "framework.h"
#include "Rendering/WindowHandler.h"
#include "Rendering/Renderer.h"
#include "Rendering/Camera.h"
#include "Log.h"
#include "InputReceiver.h"

class Application
{
private:
	static Application* s_Instance;
	Window* m_Window;
	Renderer* m_Renderer;
	Timer* m_Timer;
	InputReceiver* input;
	//Camera
	Camera m_Camera;

public:
	Application();
	~Application();

	void Run();

	inline Application& GetApplication() const { return *s_Instance; }
	inline Window& GetWindow()           const { return *m_Window;   }
	inline Renderer& GetRenderer()       const { return *m_Renderer; }

};

//This will be implemented by the client using engine lib.
Application* CreateApplication();

#endif // !1APPLICATION_H



