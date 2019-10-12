#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

#include "framework.h"
#include "Renderer.h"
#include "Timer.h"
#include <iostream>
#include "Camera.h"
#include "../System/ThreadPool.h"

/**
* A class that handles initialization of a window using OpenGL commands.
*/
class WindowClass
{

	int m_width, m_height, m_bufferWidth, m_bufferHeight;
	const char* m_title;

	Timer* m_timer;
	GLFWwindow* m_gameWindow;
	Renderer* m_renderer;		
	ThreadPool* m_threadPool;

public:
	WindowClass(const char*, int, int);
	~WindowClass();

	void Init();
	void Update();


	inline GLFWwindow* GetWindow() { return m_gameWindow; }
	inline bool WindowCloseStatus(){ return glfwWindowShouldClose(m_gameWindow); }

	//Input
	void ProcessInput(GLFWwindow* gameWindow, float deltaTime);	

};


void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif // !WINDOW_HANDLER_H

