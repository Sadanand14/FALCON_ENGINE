Setting Up the Project
GLM
When including glm in a file, ensure that you #define GLM_ENABLE_EXPERIMENTAL before the GLM includes. The basic 3D maths GLM include section would look like :

#define GLM_ENABLE_EXPERIMENTAL
#include "../glm/glm.hpp"
#include "../glm/gtx/quaternion.hpp"
GLEW
The OpenGL Extension Wrangler Library is useful for determining the level of OpenGL functionality that the target platform supports. The following guide will help enable it in your environment:

Download GLEW at : https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download

After extracting all files, move the GL folder located in glew-2.1.0\include to your project's include directory

Copy the library files in glew-2.1.0\lib\Release\x64 to your project's libraries directory

(Visual Studio) Go to your project properties, and set Configuration and Platform options to All



GLFW
The Graphics Library Framework is useful for assisting in simple window creation and input handling. The following guide will help enable it in your environment:

Download GLFW at : https://www.glfw.org/download.html

After extracting all files, move the GLFW folder to your project's include directory

Copy the library files in lib-vc2015 to your project's libraries directory

(Visual Studio) Go to your project properties, and set Configuration and Platform options to All


Boost
Note : this guide is primarily targeted at setting up a Windows development environment. You can probably just use your package manager on Linux.

Download boost at : https://www.boost.org/users/history/version_1_69_0.html

Extract into the Engine project directory

Run bootstrap.bat or bootstrap.sh depending on your development environment

Run the newly generated b2 executable (this will take a while)

(Visual Studio) Go to your project properties, and set Configuration and Platform options to All


GLM
When including glm in a file, ensure that you #define GLM_ENABLE_EXPERIMENTAL before the GLM includes. The basic 3D maths GLM include section would look like :

#define GLM_ENABLE_EXPERIMENTAL
#include "../glm/glm.hpp"
#include "../glm/gtx/quaternion.hpp"
GLEW
The OpenGL Extension Wrangler Library is useful for determining the level of OpenGL functionality that the target platform supports. The following guide will help enable it in your environment:

Download GLEW at : https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download

After extracting all files, move the GL folder located in glew-2.1.0\include to your project's include directory

Copy the library files in glew-2.1.0\lib\Release\x64 to your project's libraries directory

(Visual Studio) Go to your project properties, and set Configuration and Platform options to All

