#include "WindowsInput.h"
#include "framework.h"
#include "System/Application.h"


Input* Input::s_Instance = new WindowsInput();

bool WindowsInput::IsKeyDown(int keycode)
{
	auto window = Application::GetInstance().GetWindow().GetGLFWWindow();
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsKeyUp(int keycode)
{
	auto window = Application::GetInstance().GetWindow().GetGLFWWindow();
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_RELEASE;
}

bool WindowsInput::IsKeyRepeated(int keycode)
{
	auto window = Application::GetInstance().GetWindow().GetGLFWWindow();
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressed(int button)
{
	auto window = Application::GetInstance().GetWindow().GetGLFWWindow();
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

std::pair<float, float> WindowsInput::GetMousePositionImpl()
{
	auto window = Application::GetInstance().GetWindow().GetGLFWWindow();
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return { (float)xpos, (float)ypos };
}

float WindowsInput::GetMouseXImpl()
{
	auto [x, y] = GetMousePositionImpl();
	return x;
}

float WindowsInput::GetMouseYImpl()
{
	auto [x, y] = GetMousePositionImpl();
	return y;
}

