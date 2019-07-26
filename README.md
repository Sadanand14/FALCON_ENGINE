
## Important links:
1. [Working with premake](#precautions-to-work-with-premake-now)

2. [Build on linux](#building-falcon-on-linux)

3. [Setup Vendors and assets for the project](tools/Readme.md)



## Coding Standards

### Coding Style

Class, method, function, and other such entities are named with pascal case.

Variable names are named with camel case.

Curly braces on a new line.

`private` before `protected` before `public`; match order in source files.

Tabs for spaces, except when matching indentation.

```cpp
void Foo(u64 arg)
{
	Bar(arg,
	    true,
	    nullptr);
}
```

### Best Practices

Use define blocks instead of `#pragma once` for header guards.

```cpp
#ifndef HEADER_NAME_H_
#define HEADER_NAME_H_

// Header file contents

#endif
```

Use constructor initializater lists.

```cpp
MyClass::MyClass(u64 arg) :
	memberVariable(arg)
{
	// Possibly do something here
}
```

Use `const` and `inline` where applicable.

```cpp
inline u64 GetVar() const { return memberVariable; }
```

Limit includes as much as possible.

### Engine Preferences

Use the fixed-size numeric types defined in `types.h`.

Do not perform raw IO outside of specifically designated engine subsystems.

We will be maintaining a Linux build, with full intention of this engine supporting cross-platform development. Don't break this with platform-specific code.

## Setting Up the Project

### GLM

When including glm in a file, ensure that you `#define GLM_ENABLE_EXPERIMENTAL` before the GLM includes. The basic 3D maths GLM include section would look like :

```cpp
#define GLM_ENABLE_EXPERIMENTAL
#include "../glm/glm.hpp"
#include "../glm/gtx/quaternion.hpp"
```


### GLFW

The Graphics Library Framework is useful for assisting in simple window creation and input handling. The following guide will help enable it in your environment:

1. Download GLFW at : https://www.glfw.org/download.html

2. After extracting all files, move the `GLFW` folder to your project's `include` directory

3. Copy the library files in `lib-vc2015` to your project's `libraries` directory

4. (Visual Studio) Go to your project properties, and set Configuration and Platform options to All

5. (Visual Studio) Go to C/C++ -> General. Add `$(ProjectDir)include` to `Additional Include Directories` if it is not already there

6. (Visual Studio) Go to Linker -> General. Add `$(ProjectDir)libraries` to `Additional Library Directories` if it is not already there

7. (Visual Studio) Go to Linker -> Input. Add `glfw3.lib` to `Additional Dependencies`

### Boost

_Note : this guide is primarily targeted at setting up a Windows development environment. You can probably just use your package manager on Linux._

1. Download the zip file for boost from the team drive.

2. Extract into the "FalconEngine" project directory.

3. (Visual Studio) Go to your project properties, and set Configuration and Platform options to All.

4. (Visual Studio) Add `$(ProjectDir)boost_1_69_0` to `Include Directories`.

5. (Visual Studio) Add `$(ProjectDir)boost_1_69_0\stage\lib` to `Library Directories`.

### Building Falcon on Linux

1. To setup required libs, open a terminal and run following commands.
```shell
sudo apt-get update
sudo apt install python
sudo apt install python-dev
sudo apt install libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libglew-dev
```
2. Setting up git on linux
```shell
sudo apt install git
git config --global user.email "user@email.com"
git config --gloval user.name  "FirstName LastName"
```
Now clone the repo in your working directory.

3. Download vendor_linux.tar from drive. And extract it in the `GraphicsEngine/GraphicsEngine/` path. Use setup_vendor script for this.

4. To build and run the code,
	1. Generate project
```shell
./GenProject.sh
```
	2. Run make to build project
	```shell
	make 
	```
	if you want to clean the build directory use,
	```shell
	make clean
	```


### Precautions to work with premake now,
Whenever you add new directory in source, add any lib or any include please make sure you add it to the relevant places in the premake.lua file. otherwise there will be lot of inconsistencies. 

