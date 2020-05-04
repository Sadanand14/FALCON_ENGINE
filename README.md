## Build Sanity :

[![Build Status](https://dev.azure.com/san2889/san2889/_apis/build/status/Sadanand14.FALCON_ENGINE?branchName=master)](https://dev.azure.com/san2889/san2889/_build/latest?definitionId=2&branchName=master)

## Important links:
1. [Working with premake](#precautions-to-work-with-premake-now)

2. [Build on linux](#building-falcon-on-linux)

3. [Setup Vendors and assets for the project](tools/Readme.md)

4. [Logging in Falcon](Falcon/Falcon/System/ReadMe.md)

5. [Using Custom Allocators](Falcon/Falcon/System/Memory/ReadMe.md)

## Library Versions:
assimp - 5.0.0

boost - 1.69

glad - 0.1.30

glfw3 - 3.3

glm - 0.9.9.500

ozz - 0.10.0

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

## Setting up the project

### Generating project files:
Falcon uses premake for the as a build configuration utility. 

To generate files on Windows, you have to run `GenProject.bat` whereas on linux, you run `GenProject.sh` via terminal. 

### Downloading Vendors:

#### Manual Process:
**1. Assets:**
**Windows:**
You can download assets for windows, from [Windows Assets](https://drive.google.com/a/g.rit.edu/file/d/1lXzUblckjqBw5ZdnKEeIOGwsa4X3WoRd/view?usp=sharing). Once downloaded, unzip it under following path,
	`FALCON_ENGINE\Falcon`

**Linux:** 
You can download Linux assets from [Linux tarball](https://drive.google.com/open?id=1V2WRjAMlHCOTJoYXXKZzNuE7PgAqw7UY). Once downloaded you need to unzip these into the build directory of the project which is like,

`FALCON_ENGINE\Falcon\build\Debug\`

**2. Third party libraries/ vendor:**
	The process for vendors is same for both platforms,.You can download the specific builds for vendors from following links,
		
Windows : [vendors.zip](https://drive.google.com/open?id=1CchZegqYkadK3kZguiG7o85_ZGr8fIU9)
Linux   : [Vendor.tar.zip](https://drive.google.com/open?id=18iscICo1bDoGShJAOcVhI2Q6F5Ayr7OM)

 Once downloaded, unzip it under `FALCON_ENGINE\Falcon\Falcon\`.

#### Automated Process:
Follow the steps mentiond in [Setup Vendors and assets for the project](tools/Readme.md). 


### Building Falcon on Linux

1. To setup required libs, open a terminal and run following commands.
```shell
sudo apt-get update
sudo apt install python
sudo apt install python-dev
sudo apt install libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libglew-dev
```
2. Clone and generate project following the steps mentioned in [Setting up the project](#Setting-up-the-project).
3. To build and run the code,
```
	#2. Run make to build project
	```shell
	make 
	```
	#if you want to clean the build directory use,
	```shell
	make clean
	```
```

### Precautions to work with premake now,
Whenever you add new directory in source, add any lib or any include please make sure you add it to the relevant places in the premake.lua file. otherwise there will be lot of inconsistencies. 
