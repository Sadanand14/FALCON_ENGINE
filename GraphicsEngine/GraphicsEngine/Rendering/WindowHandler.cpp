#include "WindowHandler.h"

#include "OpenGLErrorHandler.h"
#include "Log.h"
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

	glfwMakeContextCurrent(gameWindow);
	
	
	//Creates the needed states for rendering
	renderer->CreateDrawStates();

	//sets the required states to draw the desired figure
	renderer->SetDrawStates(); 

}

void WindowClass::Update() 
{
	timer->update();
	float dt = timer->GetDeltaTime();

	//Render
	renderer->Update(dt);
	renderer->Draw(dt);

	//Swap Buffers
	glfwSwapBuffers(gameWindow);

	//Poll I/O events
	glfwPollEvents();
}

void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}
