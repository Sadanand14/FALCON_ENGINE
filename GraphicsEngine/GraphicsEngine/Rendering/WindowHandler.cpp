#include "WindowHandler.h"
#include "OpenGLErrorHandler.h"
#include "Log.h"
#include "System/Input/InputReceiver.h"




//renderer gets initialized here
Window::Window(const char* title, int width, int height ): 
	m_width(width), m_height(height), m_bufferWidth(0), m_bufferHeight(0), m_title(title)
{
	SetGLFWErrorCallback();
	Init();
}


Window::Window(int width, int height) : 
	m_width(width), m_height(height), m_bufferWidth(0), m_bufferHeight(0),m_title("Falcon")
{
	SetGLFWErrorCallback();
	Init();
}

Window::~Window() 
{
	if (m_Window) glfwDestroyWindow(m_Window);
	glfwTerminate();
}


void Window::Init() 
{
	//GLFW Configuration
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//For MacOS X
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//Create Window
	m_Window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	if (m_Window == NULL)
	{
		//Error logging will be handler by error call back method.
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_Window);
	glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
	glfwSetWindowUserPointer(m_Window, this);

	//Load OpenGL Function Pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		FL_ENGINE_ERROR( "ERROR: Failed to initialize GLAD." );
	}

	//Setting input callbacks
	SetInputCallbacks();
	
	// tell GLFW to capture our mouse
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
}



void Window::Update() 
{
	PollEvents();
	SwapBuffers();
}


void Window::SetInputCallbacks()
{
	//input callbacks
	glfwSetKeyCallback(m_Window, InputReceiver::key_callback);
	glfwSetCharCallback(m_Window, InputReceiver::char_callback);// Needed for ImGui in coming features
	glfwSetMouseButtonCallback(m_Window, InputReceiver::mouse_button_callback);
	glfwSetCursorPosCallback(m_Window, InputReceiver::cursor_callback);
	glfwSetScrollCallback(m_Window, InputReceiver::scoll_callback);
}


void Window::SetVSync(bool enable)
{
	if (enable)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	VSync = enable;
}



void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}