#ifndef OPENGLERRORHANDLER_H
#define OPENGLERRORHANDLER_H

#include <System/Log.h>
namespace GLErrorHandler {
	void glfwError(int id, const char* description)
	{
		FL_ENGINE_ERROR("ERROR:{0}, {1}", id, description);
	}
}
#endif // OPENGLERRORHANDLER_H
