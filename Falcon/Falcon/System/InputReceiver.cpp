#include "InputReceiver.h"

#include <iostream>
#include "Log.h"

boost::array<bool, MAX_KEYS> InputReceiver::m_keyStates = {false};
boost::array<bool, MAX_MOUSE_KEYS> InputReceiver::m_mouseStates = { false };
double InputReceiver::mouse_x = 0, InputReceiver::mouse_y = 0;


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
	//glfwSetKeyCallback(window, key_callback);
	//glfwSetMouseButtonCallback(window, mouse_callback);
	//glfwSetCursorPosCallback(window, cursor_callback);
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

	//FL_ENGINE_INFO(whatever is being printed in the next line)
	//std::cout << "A key int: " << GLFW_KEY_A << "\n key pressed state for A(" << key<<") is: "<< m_keyStates[key] <<std::endl; 

	FL_ENGINE_INFO("INFO: {0}, Key pressed state for A({1}) is: {2}", GLFW_KEY_A,key, m_keyStates[key]);
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
	mouse_x = xpos;
	mouse_y = ypos;
}

