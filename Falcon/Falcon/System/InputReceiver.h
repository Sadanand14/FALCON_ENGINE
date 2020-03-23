#ifndef INPUT_RECEIVER_H
#define INPUT_RECEIVER_H

#define MAX_KEYS 500
#define MAX_MOUSE_KEYS 32

#include <glm/glm.hpp>
#include <Events/KeyEvents.h>
#include <Events/EventManager.h>
#include <WindowHandler.h>
#include "Log.h"
#include <boost/array.hpp>


class InputReceiver 
{

private:
	static bool m_keyStates[MAX_KEYS], m_prevStates[MAX_KEYS];
	static bool m_mouseStates[MAX_MOUSE_KEYS];
	static double m_mouseX, m_mouseY, m_scrollX, m_scrollY;

public:

	InputReceiver(WindowClass* window);
	~InputReceiver();

	static inline glm::vec2 GetScroll() { return glm::vec2(m_scrollX,m_scrollY); }
	static inline glm::vec2 GetCursor() { return glm::vec2(m_mouseX, m_mouseY); }
	static void Update();
	static bool GetKey(int keyValue);
	static bool GetKeyPress(int keyValue);
	static bool GetKeyRelease(int keyValue);
	static void Init(GLFWwindow*);
private:


	static void key_callback(GLFWwindow*, int, int, int, int);
	static void mouse_callback(GLFWwindow*, int, int, int);
	static void cursor_callback(GLFWwindow*, double, double);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif // !1
