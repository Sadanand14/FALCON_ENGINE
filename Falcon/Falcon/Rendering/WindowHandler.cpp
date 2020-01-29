#include "WindowHandler.h"
#include "OpenGLErrorHandler.h"
#include "Log.h"
#include "Memory/fmemory.h"
#include <string>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

//renderer gets initialized here
WindowClass::WindowClass(const char* title, int width, int height ): m_width(width), m_height(height), m_title(title)
{

	glfwSetErrorCallback(&GLErrorHandler::glfwError);
	Init();
	glfwSetErrorCallback(&GLErrorHandler::glfwError);

}

WindowClass::~WindowClass()
{
	if (m_gameWindow) { glfwDestroyWindow(m_gameWindow);  }

	glfwTerminate();
}


void WindowClass::Init()
{
	m_threadPool = ThreadPool::GetThreadPool();
	//GLFW Configuration
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//For MacOS X
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//Create Window
	m_gameWindow = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	if (m_gameWindow == NULL)
	{
		//Error logging will be handler by error call back method.
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_gameWindow);
	glfwSwapInterval(0);
	glfwSetFramebufferSizeCallback(m_gameWindow, framebuffer_size_callback);
	glfwSetWindowUserPointer(m_gameWindow, this);

	//Load OpenGL Function Pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		FL_ENGINE_ERROR( "ERROR: Failed to initialize GLAD." );
	}

	// tell GLFW to capture our mouse
	glfwSetInputMode(m_gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	ctx = nk_glfw3_init(m_gameWindow, NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

