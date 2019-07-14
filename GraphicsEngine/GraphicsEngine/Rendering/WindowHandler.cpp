#include "WindowHandler.h"
#include "OpenGLErrorHandler.h"
#include "Log.h"

//Camera 
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//Camera Setup	
float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

//renderer gets initialized here
WindowClass::WindowClass(const char* title, int width, int height ): m_width(width), m_height(height), m_title(title)
{
	m_renderer = new Renderer(); // creates a new renderer class on the heap
	m_timer = new Timer(); // creates a new timer class in the heap
	glfwSetErrorCallback(&GLErrorHandler::glfwError);
	Init();
}

WindowClass::~WindowClass() 
{
	delete m_timer;
	delete m_renderer;
	if (m_gameWindow) glfwDestroyWindow(m_gameWindow);
	glfwTerminate();
}


void WindowClass::Init() 
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
	m_gameWindow = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	if (m_gameWindow == NULL)
	{
		//Error logging will be handler by error call back method.
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_gameWindow);
	glfwSetFramebufferSizeCallback(m_gameWindow, framebuffer_size_callback);
	glfwSetWindowUserPointer(m_gameWindow, this);

	//Load OpenGL Function Pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		FL_ENGINE_ERROR( "ERROR: Failed to initialize GLAD." );
	}

	//Camera
	glfwSetCursorPosCallback(m_gameWindow, mouse_callback);
	glfwSetScrollCallback(m_gameWindow, scroll_callback);
	
	// tell GLFW to capture our mouse
	glfwSetInputMode(m_gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	//Create Draw States in Renderer
	m_renderer->CreateDrawStates();

	//Set Draw States in Renderer
	m_renderer->SetDrawStates();
}

void WindowClass::Update() 
{
	while (!glfwWindowShouldClose(m_gameWindow))
	{
		m_timer->update();
		float dt = m_timer->GetDeltaTime();

		//Game Input
		ProcessInput(m_gameWindow, dt);

		//Camera
		glm::mat4 view = camera.GetViewMatrix();

		//Render
		m_renderer->Update(m_width, m_height, camera.m_Zoom, view, dt);
		m_renderer->Draw();

		//Swap Buffers
		glfwSwapBuffers(m_gameWindow);

		//Poll I/O events
		glfwPollEvents();
	}
}

void WindowClass::ProcessInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}