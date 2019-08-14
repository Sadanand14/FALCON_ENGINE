#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

#include "framework.h"
#include "Renderer.h"
#include "Timer.h"
#include <iostream>
#include "Camera.h"
#include "events/Event.h"
#include "OpenGLErrorHandler.h"
using EventCallbackFunc = std::function<void(events::Event&)>;


class Window
{
	GLFWwindow* m_Window;

	// Following data could be moved out to a struct which holds Windows Metadata
	int m_width, m_height, m_bufferWidth, m_bufferHeight;
	const char* m_title;	
	bool VSync;
	EventCallbackFunc m_EventCallback;

	void SetInputCallbacks();

	void SetGLFWErrorCallback() const;

public:
	Window(const char*, int, int);
	Window(int width, int height);


	~Window();

	void Init();
	void Update();

	inline GLFWwindow* GetWindow() { return m_Window; }
	
	inline bool GetWindowShouldClose() const { return glfwWindowShouldClose(m_Window); }
	inline void SetWindowShouldClose(bool val) { glfwSetWindowShouldClose(m_Window, val);}

	inline void SwapBuffers() const { glfwSwapBuffers(m_Window); }

	inline void PollEvents() const { glfwPollEvents(); }

	inline const int GetWidth() const { return m_width; }
	
	inline const int GetHeight() const { return m_height; }

	inline void SetWindowsEventCallbackFunction(const EventCallbackFunc& callback) { m_EventCallback = callback; }
	inline EventCallbackFunc GetWindowEventCallbackFunction() const { return m_EventCallback; }



	void SetVSync(bool enable);
	inline bool isVSync() const { return VSync; }

	inline GLFWwindow* GetGLFWWindow() const { return m_Window; }

};

void frame_buffer_size_callback(GLFWwindow*, int, int);


#endif // !WINDOW_HANDLER_H

