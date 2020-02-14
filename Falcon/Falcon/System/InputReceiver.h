#ifndef INPUT_RECEIVER_H
#define INPUT_RECEIVER_H

#define MAX_KEYS 500
#define MAX_MOUSE_KEYS 32

#include <Events/KeyEvents.h>
#include <Events/EventManager.h>
#include <WindowHandler.h>
#include "Log.h"
#include <boost/array.hpp>


class InputReceiver 
{

private:
	static boost::array<bool, MAX_KEYS> m_keyStates, m_prevStates;
	static boost::array<bool, MAX_MOUSE_KEYS> m_mouseStates;
	static double mouse_x, mouse_y;

public:

	InputReceiver(WindowClass* window);
	~InputReceiver();

	static void Update();
	static bool GetKey(int keyValue);
	static bool GetKeyPress(int keyValue);
	static bool GetKeyRelease(int keyValue);
	static void Init(GLFWwindow*);
private:


	static void key_callback(GLFWwindow*, int, int, int, int);
	static void mouse_callback(GLFWwindow*, int, int, int);
	static void cursor_callback(GLFWwindow*, double, double);
};

#endif // !1
