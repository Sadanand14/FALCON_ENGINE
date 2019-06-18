#include "WidnowHandler.h"

#include "OpenGLErrorHandler.h"


WindowClass::WindowClass(const char* title, int width, int height )
{
	m_title = title;
	m_height = height;
	m_width = width;
	glfwSetErrorCallback(&GLErrorHandler::glfwError);
	Init();
}

WindowClass::~WindowClass() 
{
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
	gameWindow = glfwCreateWindow(m_width, 200, m_title, glfwGetPrimaryMonitor(), NULL);
	if (gameWindow == NULL)
	{
		//Error logging will be handler by error call back method.
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(gameWindow);
	glfwSetFramebufferSizeCallback(gameWindow, framebuffer_size_callback);
	//glViewport(0, 0, bufferWidth, bufferHeight);
	//Load OpenGL Function Pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
	}

	glfwMakeContextCurrent(gameWindow);


	/*glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		glewInitialized = true;
		glfwDestroyWindow(gameWindow);
		glfwTerminate();
		return;
	}*/

	return;
}

void WindowClass::Update() 
{
	if (!glfwWindowShouldClose(gameWindow))
	{
		//Input
		gameInput(gameWindow);

		//Render
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Swap Buffers
		glfwSwapBuffers(gameWindow);

		//Poll I/O events
		glfwPollEvents();
	}
}

void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void gameInput(GLFWwindow* gameWindow)
{
	if (glfwGetKey(gameWindow, GLFW_KEY_ESCAPE) != GLFW_RELEASE)
		glfwSetWindowShouldClose(gameWindow, true);
}