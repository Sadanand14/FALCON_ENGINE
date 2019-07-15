workspace "GraphicsEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}

IncludeDirs["vendor"] = "GraphicsEngine/vendor/include/";
IncludeDirs["spdlog"] = "GraphicsEngine/vendor/include/spdlog";
IncludeDirs["GLFW"]   = "GraphicsEngine/vendor/include/glfw";
IncludeDirs["glad"]   = "GraphicsEngine/vendor/include/glad";
IncludeDirs["assimp"] = "GraphicsEngine/vendor/include/assimp";
IncludeDirs["KHR"]    = "GraphicsEngine/vendor/include/KHR";
IncludeDirs["boost"]  = "GraphicsEngine/vendor/include/boost";


LinkDebugDirs = {}
LinkDebugDirs["assimp"] = "GraphicsEngine/vendor/libs/assimp/Debug"
LinkDebugDirs["boost"]  = "GraphicsEngine/vendor/libs/boost"
LinkDebugDirs["GLFW"]   = "GraphicsEngine/vendor/libs/GLFW/Debug"

LinkReleaseDirs = {}
LinkReleaseDirs["assimp"] = "GraphicsEngine/vendor/libs/assimp/Release"
LinkReleaseDirs["boost"]  = "GraphicsEngine/vendor/libs/boost"
LinkReleaseDirs["GLFW"]   = "GraphicsEngine/vendor/libs/GLFW/Release"



printf("%s",LinkReleaseDirs.assimp)


project "GraphicsEngine"
	location "GraphicsEngine"
	kind "ConsoleApp"
	language "C++"


	-- SETTING UP PATHS FOR BUILD DIRS--
	targetdir ("build/" .. outputdir .. "/bin/%{prj.name}")
	objdir    ("build/" .. outputdir .. "/intermediates/%{prj.name}")

	
	--SETTING UP BASIC FILES CONTENT AND INCLUDE DIRS--
	files
	{
		"%{prj.name}/System/**.h",
		"%{prj.name}/System/**.hpp",
		"%{prj.name}/System/**.cpp",
		"%{prj.name}/System/**.c",
		"%{prj.name}/Rendering/**.h",
		"%{prj.name}/Rendering/**.hpp",
		"%{prj.name}/Rendering/**.cpp",
		"%{prj.name}/Shader/**.vert",
		"%{prj.name}/Shader/**.frag"
	}

	includedirs
	{
		"%{prj.name}",
		"%{prj.name}/System",
		"%{IncludeDirs.vendor}",
		"%{IncludeDirs.glad}",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.KHR}",
		"%{IncludeDirs.boost}",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.assimp}",
	}
	

	---SETTING UP THINGS FOR windows--
	filter "system:windows"
		cppdialect "C++17"
		--staticruntime "On"
		systemversion "latest"

		defines
		{
			"FL_PLATFORM_WINDOWS"
		}

		links 
		{
			"glfw3.lib",
			"opengl32.lib"
		}


	filter { "system:windows", "configurations:Debug" }
			defines "BUILD_DEBUG_MODE"
			symbols "On"
     		
			libdirs
			{
				"%{LinkDebugDirs.GLFW}",
				"%{LinkDebugDirs.boost}",
				"%{LinkDebugDirs.assimp}",
			}

	
	filter { "system:windows", "configurations:Release" }
     		
			defines "BUILD_RELEASE_MODE"
			optimize "On"

			libdirs
			{
				"%{LinkReleaseDirs.GLFW}",
				"%{LinkReleaseDirs.boost}",
				"%{LinkReleaseDirs.assimp}"
			}

	
	--------SETTING UP THINGS FOR LINUX
	
	filter "system:linux"
		pic "On"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"FL_PLATFORM_UNIX"
		}

		links 
		{
			"Xrandr",
	        "Xi",
			"glfw3",
        	"GLEW",
            "GLU",
            "GL",
            "X11",
			"dl",
			"pthread"
		}
	


	filter {"system:linux","configurations:Debug"}
		defines "BUILD_DEBUG_MODE"
		symbols "On"

		printf("%s",LinkReleaseDirs.assimp)

		libdirs
		{
			"%{LinkDebugDirs.GLFW}",
			"%{LinkDebugDirs.boost}",
			"%{LinkDebugDirs.assimp}",
			"/usr/local/lib",
			"/usr/lib"
		}

	filter {"system:linux","configurations:Release"}
		defines "BUILD_RELEASE_MODE"
		optimize "On"


		libdirs
		{
			"%{LinkReleaseDirs.GLFW}",
			"%{LinkReleaseDirs.boost}",
			"%{LinkReleaseDirs.assimp}",
			"/usr/local/lib",
			"/usr/lib"
		}