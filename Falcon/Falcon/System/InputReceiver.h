#ifndef INPUT_RECEIVER_H
#define INPUT_RECEIVER_H

#define MAX_KEYS 500
#define MAX_MOUSE_KEYS 32

#include <Core/KeyEvents.h>
#include <Core/EventManager.h>
#include <Rendering/WindowHandler.h>
#include "Log.h"
#include <boost/array.hpp>


class InputReceiver 
{

private:
	static boost::array<bool, MAX_KEYS> m_keyStates;
	static boost::array<bool, MAX_MOUSE_KEYS> m_mouseStates;
	static double mouse_x, mouse_y;

public:

	InputReceiver(WindowClass* window);
	~InputReceiver();
	
private:

	void Init(GLFWwindow*);

	//static void key_callback(GLFWwindow*, int, int, int, int);
	//static void mouse_callback(GLFWwindow*, int, int, int);
	//static void cursor_callback(GLFWwindow*, double, double);
};

#endif // !1
