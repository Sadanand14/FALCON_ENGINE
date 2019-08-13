#ifndef OPENGLERRORHANDLER_H
#define OPENGLERRORHANDLER_H

#include "Log.h"
#include "framework.h"


namespace GLErrorHandler {
	void glfwError(int id, const char* description);

	void GLAPIENTRY MessageCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam);

}
#endif // OPENGLERRORHANDLER_H