#ifndef INPUT_RECEIVER_H
#define INPUT_RECEIVER_H

#define MAX_KEYS 500
#define MAX_MOUSE_KEYS 32

#include "../Rendering/WindowHandler.h"
#include "Log.h"
#include <boost/array.hpp>

class InputReceiver 
{

private:
	static boost::array<bool, MAX_KEYS> s_keyStates;
	static boost::array<bool, MAX_MOUSE_KEYS> s_mouseStates;
	static double s_xpos, s_ypos;
	static double s_xchanged, s_ychanged;

public:

	static void key_callback(GLFWwindow*, int, int, int, int);
	static void char_callback(GLFWwindow*, unsigned int);
	static void mouse_button_callback(GLFWwindow*, int, int, int);
	static void cursor_callback(GLFWwindow*, double, double);
	static void scoll_callback(GLFWwindow*, double, double);

};

#endif // INPUT_RECEIVER_H
