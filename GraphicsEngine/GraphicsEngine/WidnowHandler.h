#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

#include "framework.h"
#include <iostream>

class WindowClass
{
	int m_width, m_height, bufferWidth, bufferHeight;
	const char* m_title;
	GLFWwindow* gameWindow;

public:

	WindowClass(const char*, int, int);
	~WindowClass();

	void Init();
	void Update();
};

void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height);
void gameInput(GLFWwindow* gameWindow);

#endif // !WINDOW_HANDLER_H

