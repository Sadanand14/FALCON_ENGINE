#ifndef APPLICATION_H
#define APPLICATION_H

#include "framework.h"
#include "Rendering/WindowHandler.h"
#include "Rendering/Renderer.h"

class Application
{

private:
	static Application* s_Instance;
	Window* m_Window;
	Renderer* m_Renderer;
	
public:
	static inline Application& GetInstance() { return *s_Instance; }
	Application();
	
	virtual ~Application();
	void Run();
	inline const Window& GetWindow() const { return *m_Window; }
};

//To be defined by client
Application* CreateApplication();



#endif // !APPLICATION_H