#include "InputReceiver.h"
#include "Log.h"
#include "System/events/ApplicationEvent.h"
#include "System/events/KeyEvent.h"
#include "System/events/MouseEvent.h"
#include "Rendering/WindowHandler.h"

boost::array<bool, MAX_KEYS> InputReceiver::s_keyStates = {false};
boost::array<bool, MAX_MOUSE_KEYS> InputReceiver::s_mouseStates = { false };
double InputReceiver::s_xpos = 0.0, InputReceiver::s_ypos = 0.0;

InputReceiver::InputReceiver(Window* windowClass) 
{
	Init(windowClass->GetWindow());
}

InputReceiver::~InputReceiver() 
{
	
}


void InputReceiver::key_callback(GLFWwindow* glfwWindow, int keyCode, int scancode, int action, int mods)
{
	//Get the current active window and associated callback function.
	Window& window = *(static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow)));
	EventCallbackFunc eventCallBackFunc = window.GetWindowEventCallbackFunction();

	//Create appropriate event based on the action.
	switch (action)
	{
		case GLFW_PRESS:
		{
			events::KeyPressedEvent event(keyCode, 0);
			eventCallBackFunc(event);
			break;
		}
		
		case GLFW_REPEAT:
		{
			events::KeyPressedEvent event(keyCode, 1);
			eventCallBackFunc(event);
			break;
		}

		case GLFW_RELEASE:
		{
			events::KeyReleasedEvent event(keyCode);
			eventCallBackFunc(event);
			break;
		}
	}
}

void InputReceiver::char_callback(GLFWwindow* glfwWindow, unsigned int keyCode)
{
	//Get the current active window and associated callback function.
	Window& window = *(static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow)));
	EventCallbackFunc eventCallBackFunc = window.GetWindowEventCallbackFunction();
	events::KeyTypedEvent event(keyCode);
	EventCallbackFunc(event);
}


void InputReceiver::mouse_button_callback(GLFWwindow* glfwWindow, int button, int action, int mods)
{
	//Get the current active window and associated callback function.
	Window& window = *(static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow)));
	EventCallbackFunc eventCallBackFunc = window.GetWindowEventCallbackFunction();

	//Create appropriate event based on the action.
	switch (action)
	{
		case GLFW_PRESS:
		{
			events::MouseButtonPressedEvent event(button);
			eventCallBackFunc(event);
			break;
		}


		case GLFW_RELEASE:
		{
			events::MouseButtonReleasedEvent event(button);
			eventCallBackFunc(event);
			break;
		}
	}
}

void InputReceiver::cursor_callback(GLFWwindow* glfwWindow, double xpos, double ypos)
{
	//Get the current active window and associated callback function.
	Window& window = *(static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow)));
	EventCallbackFunc eventCallBackFunc = window.GetWindowEventCallbackFunction();
	events::MouseMovedEvent event((float)xpos, (float)ypos);
	eventCallBackFunc(event);
}

void InputReceiver::scoll_callback(GLFWwindow* glfwWindow, double xOffset, double yOffset)
{
	//Get the current active window and associated callback function.
	Window& window = *(static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow)));
	EventCallbackFunc eventCallBackFunc = window.GetWindowEventCallbackFunction();
	events::MouseScrolledEvent event((float)xOffset, (float)yOffset);
	eventCallBackFunc(event);
}

