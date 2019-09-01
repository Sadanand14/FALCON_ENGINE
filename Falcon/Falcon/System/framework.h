// header.h : include file for standard system include files,
// or project specific include files

#ifndef FRAMEWORK_H
#define FRAMEWORK_H




#ifdef FL_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files
#include "targetver.h"
#include <windows.h>
#include <tchar.h>

#endif


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

//OpenGL specific files
#include "glad/glad.h"
#include "glfw/glfw3.h"

#endif //!FRAMEWORK_H
