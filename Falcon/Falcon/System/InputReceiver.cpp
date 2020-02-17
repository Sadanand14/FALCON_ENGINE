#include "InputReceiver.h"

#include <iostream>
#include "Log.h"

boost::array<bool, MAX_KEYS> InputReceiver::m_prevStates = {false};
boost::array<bool, MAX_KEYS> InputReceiver::m_keyStates = {false};
boost::array<bool, MAX_MOUSE_KEYS> InputReceiver::m_mouseStates = { false };
double InputReceiver::m_mouseX = 0, InputReceiver::m_mouseY= 0, InputReceiver::m_scrollX = 0 , InputReceiver::m_scrollY = 0;


/**
* Main Constructor for the input class.
*
*@param[in] A pointer to an object of Window Class type.
*/
InputReceiver::InputReceiver(WindowClass* windowClass) 
{
	Init(windowClass->GetWindow());
}


/**
* Destructor for the Input Class.
*/
InputReceiver::~InputReceiver() 
{
	
}


/**
*Initializer function for the input class.
*
*@param[in] A pointer to an object of GLFWwindow type.
*/
void InputReceiver::Init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetScrollCallback(window, scroll_callback);
}


void InputReceiver::Update() 
{
	for (unsigned int i = 0; i < MAX_KEYS; ++i) 
	{
		m_prevStates[i] = m_keyStates[i];
	}
	//m_scrollX = 0;
	//m_scrollY = 0;
}

bool InputReceiver::GetKey(int key) 
{
	if (m_prevStates[key] == m_keyStates[key] && m_prevStates[key]) 
	{
		return true;
	} 
	return false;
}

bool InputReceiver::GetKeyPress(int key) 
{
	if (m_keyStates[key] && !m_prevStates[key]) 
	{
		return true; 
	}
		
	return false;
}

bool InputReceiver::GetKeyRelease(int key) 
{
	if (!m_keyStates[key] && m_prevStates[key]) 
	{
		return true;
	}
	return false;
}


/**
* Key Callback for Input Class. Gets Triggered when Key is pressed.	
*
*@param[in] A pointer to an object of GLFWwindow type.
*@param[in] int type data for relating to a Key.
*@param[in] int type data relating to a scan code.
*@param[in] int type data for relating to an action.
*@param[in] int type data for relating to a mod.
*/
void InputReceiver::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	m_keyStates[key] = (action != GLFW_RELEASE);
}

/**
*Mouse Class back for the Input class. Gets triggered when Mouse buttons are clicked.
*
*@param[in] A pointer to an object of GLFWwindow type.
*@param[in] int type data for relating to a mouse button.
*@param[in] int type data for relating to an action.
*@param[in] int type data for relating to a mod.
*/
void InputReceiver::mouse_callback(GLFWwindow* window, int button, int action, int mods) 
{
	m_mouseStates[button] = (action==GLFW_PRESS);
}

/**
* Cursor Call back for the Input class. Gets Triggered every time the mouse cursor gets moved on screeen.
* 
*@param[in] A pointer to an object of GLFWwindow type.
*@param[in] A double precision float for x-position of cursor on screen.
*@param[in] A double precision float for y-position of cursor on screen.
*/
void InputReceiver::cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	m_mouseX = xpos;
	m_mouseY = ypos;
}


void InputReceiver::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_scrollX = xoffset;
	m_scrollY = yoffset;
}
