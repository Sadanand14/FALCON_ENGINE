#include "InputReceiver.h"

#include <iostream>
#include "Log.h"

boost::array<bool, MAX_KEYS> InputReceiver::m_keyStates = {false};
boost::array<bool, MAX_MOUSE_KEYS> InputReceiver::m_mouseStates = { false };
double InputReceiver::mouse_x = 0, InputReceiver::mouse_y = 0;

InputReceiver::InputReceiver(WindowClass* windowClass) 
{
	Init(windowClass->GetWindow());
}

InputReceiver::~InputReceiver() 
{
	
}

void InputReceiver::Init(GLFWwindow* window)
{
	//glfwSetKeyCallback(window, key_callback);
	//glfwSetMouseButtonCallback(window, mouse_callback);
	//glfwSetCursorPosCallback(window, cursor_callback);
}

void InputReceiver::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	m_keyStates[key] = (action != GLFW_RELEASE);

	//FL_ENGINE_INFO(whatever is being printed in the next line)
	//std::cout << "A key int: " << GLFW_KEY_A << "\n key pressed state for A(" << key<<") is: "<< m_keyStates[key] <<std::endl; 

	FL_ENGINE_INFO("INFO: {0}, Key pressed state for A({1}) is: {2}", GLFW_KEY_A,key, m_keyStates[key]);
}

void InputReceiver::mouse_callback(GLFWwindow* window, int button, int action, int mods) 
{
	m_mouseStates[button] = (action==GLFW_PRESS);
}

void InputReceiver::cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouse_x = xpos;
	mouse_y = ypos;
}

