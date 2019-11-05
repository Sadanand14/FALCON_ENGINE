#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

<<<<<<< HEAD
#include <framework.h>


#include <Timer.h>
=======
#include "framework.h"
#include "Renderer.h"
#include "Timer.h"
#include "Core/Scene.h"

>>>>>>> 40b9d8aa0cb907f7a410bbfc542de38254fe9fbc
#include <iostream>

#include <ThreadPool.h>

/**
* A class that handles initialization of a window using OpenGL commands.
*/
class WindowClass
{

	int m_width, m_height, m_bufferWidth, m_bufferHeight;
	const char* m_title;

	GLFWwindow* m_gameWindow;
<<<<<<< HEAD
	
=======
	Renderer* m_renderer;
>>>>>>> 40b9d8aa0cb907f7a410bbfc542de38254fe9fbc
	ThreadPool* m_threadPool;

public:
	WindowClass(const char*, int, int);
	~WindowClass();

	void Init();

	inline const unsigned int GetWidth()const { return m_width; }
	inline const unsigned int GetHeight()const { return m_height; }

	inline void SetTitle() {};
	inline GLFWwindow* GetWindow() { return m_gameWindow; }
	inline bool WindowCloseStatus(){ return glfwWindowShouldClose(m_gameWindow); }

<<<<<<< HEAD
=======
	//Input
	void ProcessInput(GLFWwindow* gameWindow, float deltaTime);

>>>>>>> 40b9d8aa0cb907f7a410bbfc542de38254fe9fbc
};

void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height); 

#endif // !WINDOW_HANDLER_H

