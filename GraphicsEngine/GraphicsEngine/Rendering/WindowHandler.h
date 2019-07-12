#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

#include "framework.h"
#include "Renderer.h"
#include "Timer.h"
#include <iostream>
#include "..//System/Camera.h"

class WindowClass
{

	int m_width, m_height, bufferWidth, bufferHeight;
	const char* m_title;

	Timer* timer;
	GLFWwindow* gameWindow;
	Renderer* renderer;	

	

public:
	WindowClass(const char*, int, int);
	~WindowClass();

	void Init();
	void Update();


	inline GLFWwindow* GetWindow() { return gameWindow; }
	inline bool WindowCloseStatus(){ return glfwWindowShouldClose(gameWindow); }

	//Input
	void processInput(GLFWwindow* gameWindow, float deltaTime);	

};

void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);





#endif // !WINDOW_HANDLER_H

