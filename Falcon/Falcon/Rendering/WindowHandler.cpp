#include "WindowHandler.h"
#include "OpenGLErrorHandler.h"
#include "Log.h"
#include "Application.h"

//Camera Setup	
float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

Window::Window(const char* title, int width, int height ): m_width(width), m_height(height), m_bufferWidth(0), m_bufferHeight(0),m_title(title)
{
	glfwSetErrorCallback(&GLErrorHandler::glfwError);
	Init();
	SetVSync(true);
}

Window::Window(int width, int height) : m_width(width), m_height(height), m_bufferWidth(0), m_bufferHeight(0), m_title("Falcon")
{
	glfwSetErrorCallback(&GLErrorHandler::glfwError);
	Init();
	SetVSync(true);
}

Window::~Window() 
{
	if (m_GLWindow) glfwDestroyWindow(m_GLWindow);
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
	m_GLWindow = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	if (m_GLWindow == NULL)
	{
		//Error logging will be handler by error call back method.
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_GLWindow);
	glfwSetFramebufferSizeCallback(m_GLWindow, framebuffer_size_callback);
	glfwSetWindowUserPointer(m_GLWindow, this);

	//Load OpenGL Function Pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		FL_ENGINE_ERROR( "ERROR: Failed to initialize GLAD." );
	}

	//Camera
	glfwSetCursorPosCallback(m_GLWindow, mouse_callback);
	glfwSetScrollCallback(m_GLWindow, scroll_callback);
	
	// tell GLFW to capture our mouse
	glfwSetInputMode(m_GLWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
}

void Window::Update()
{
	SwapBuffers();
	PollEvents();
}


void Window::ProcessInput(GLFWwindow* window, float deltaTime)
{
	Camera* camera = Application::GetApplication().GetCamera();
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(FORWARD, deltaTime);
		FL_ENGINE_INFO("I am processign input!!");
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);
}


void Window::SetVSync(bool enable)
{
	if (enable)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_VSync = enable;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Camera* camera = Application::GetApplication().GetCamera();
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera* camera = Application::GetApplication().GetCamera();
	camera->ProcessMouseScroll((float)yoffset);
}

void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}