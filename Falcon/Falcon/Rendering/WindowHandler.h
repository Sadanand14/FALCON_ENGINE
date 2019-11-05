#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

#include <framework.h>


#include <Timer.h>
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

};

void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height); 

#endif // !WINDOW_HANDLER_H

