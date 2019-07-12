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
	renderer = new Renderer(); // creates a new renderer class on the heap
	timer = new Timer(); // creates a new timer class in the heap
	glfwSetErrorCallback(&GLErrorHandler::glfwError);
	Init();
}

WindowClass::~WindowClass() 
{
	delete timer;
	delete renderer;
	if (gameWindow) glfwDestroyWindow(gameWindow);
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
	gameWindow = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	if (gameWindow == NULL)
	{
		//Error logging will be handler by error call back method.
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(gameWindow);
	glfwSetFramebufferSizeCallback(gameWindow, framebuffer_size_callback);
	glfwSetWindowUserPointer(gameWindow, this);

	//Load OpenGL Function Pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		FL_ENGINE_ERROR( "ERROR: Failed to initialize GLAD." );
	}

	//Camera
	glfwSetCursorPosCallback(gameWindow, mouse_callback);
	glfwSetScrollCallback(gameWindow, scroll_callback);
	
	// tell GLFW to capture our mouse
	glfwSetInputMode(gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	//Create Draw States in Renderer
	renderer->CreateDrawStates();

	//Set Draw States in Renderer
	renderer->SetDrawStates();
}

void WindowClass::Update() 
{
	while (!glfwWindowShouldClose(gameWindow))
	{
		timer->update();
		float dt = timer->GetDeltaTime();

		//Game Input
		processInput(gameWindow, dt);

		//Camera
		glm::mat4 view = camera.GetViewMatrix();

		//Render
		renderer->Update(m_width, m_height, camera.Zoom, view, dt);
		renderer->Draw();

		//Swap Buffers
		glfwSwapBuffers(gameWindow);

		//Poll I/O events
		glfwPollEvents();
	}
}

void WindowClass::processInput(GLFWwindow* window, float deltaTime)
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