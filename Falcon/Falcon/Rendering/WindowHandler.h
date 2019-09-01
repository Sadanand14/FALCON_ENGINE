#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

#include "framework.h"
#include "Renderer.h"
#include "Timer.h"
#include <iostream>
#include "Camera.h"

class Window
{

	int m_width, m_height, m_bufferWidth, m_bufferHeight;
	const char* m_title;
	bool m_VSync;
	GLFWwindow* m_GLWindow;		

public:
	Window(const char*, int, int);
	Window( int, int);
	~Window();

	void Init();
	void Update();
	void SetVSync(bool);
	
	inline const int GetWidth() const { return m_width; }

	inline const int GetHeight() const { return m_height; }

	inline GLFWwindow* GetGLFWWindow() { return m_GLWindow; }
	
	inline bool GetWindowShouldClose(){ return glfwWindowShouldClose(m_GLWindow); }

	inline void SwapBuffers() const { glfwSwapBuffers(m_GLWindow); }

	inline void PollEvents() const { glfwPollEvents(); }


	//Input
	void ProcessInput(GLFWwindow* window, float deltaTime);	

};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);





#endif // !WINDOW_HANDLER_H

