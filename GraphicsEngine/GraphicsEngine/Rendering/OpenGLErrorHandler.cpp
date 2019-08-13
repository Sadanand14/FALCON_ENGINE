#include "OpenGLErrorHandler.h"

void GLErrorHandler::glfwError(int id, const char* description)
{
	FL_ENGINE_ERROR("ERROR:{0}, {1}", id, description);
}




void GLAPIENTRY GLErrorHandler::MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	/*fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);*/

	if(type == GL_DEBUG_TYPE_ERROR)
		FL_ENGINE_INFO("GL ERROR:type = {0}, severity = {1}, message ={2}\n",
			type, severity, message);
	
}