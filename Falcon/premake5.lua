workspace "Falcon"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}

IncludeDirs["vendor"] = "Falcon/vendor/include/";
IncludeDirs["spdlog"] = "Falcon/vendor/include/spdlog";
IncludeDirs["GLFW"]   = "Falcon/vendor/include/glfw";
IncludeDirs["glad"]   = "Falcon/vendor/include/glad";
IncludeDirs["assimp"] = "Falcon/vendor/include/assimp";
IncludeDirs["KHR"]    = "Falcon/vendor/include/KHR";
IncludeDirs["boost"]  = "Falcon/vendor/include/boost";


LinkDebugDirs = {}
LinkDebugDirs["assimp"] = "Falcon/vendor/libs/assimp/Debug"
LinkDebugDirs["boost"]  = "Falcon/vendor/libs/boost"
LinkDebugDirs["GLFW"]   = "Falcon/vendor/libs/GLFW/Debug"

LinkReleaseDirs = {}
LinkReleaseDirs["assimp"] = "Falcon/vendor/libs/assimp/Release"
LinkReleaseDirs["boost"]  = "Falcon/vendor/libs/boost"
LinkReleaseDirs["GLFW"]   = "Falcon/vendor/libs/GLFW/Release"



printf("%s",LinkReleaseDirs.assimp)


project "Falcon"
	location "Falcon"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"


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

		--[=====[links 
		{
			"glfw3.lib",
			"opengl32.lib";
		}
		--]=====]
		nuget {'glm:0.9.9.500'}


	

	filter { "system:windows", "configurations:Debug" }
			defines "BUILD_DEBUG_MODE"
			symbols "On"
     		
			--[=====[links
			{
				"assimp-vc142-mtd.lib"
			}
			--]=====]

			libdirs
			{
				"%{LinkDebugDirs.GLFW}",
				"%{LinkDebugDirs.boost}",
				"%{LinkDebugDirs.assimp}",
			}


	
	filter { "system:windows", "configurations:Release" }
     		
			defines "BUILD_RELEASE_MODE"
			optimize "On"

		--[=====[ 	links
			{
				"assimp-vc142-mt.lib"
			}
			--]=====]
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
			"pthread",
			"assimp"
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


	--Setting up prebuild commands--
	
	filter{"configurations:Debug"}
		assimp_abs_path_deb = path.getabsolute(LinkDebugDirs["assimp"])
	
		postbuildcommands ('{COPY} "%{assimp_abs_path_deb}" "%{cfg.targetdir}"')

	filter{"configurations:Release"}
		assimp_abs_path_release = path.getabsolute(LinkReleaseDirs["assimp"])
	
		postbuildcommands ('{COPY} "%{assimp_abs_path_release}" "%{cfg.targetdir}"')


--Game Project Config Setup--

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	-- SETTING UP PATHS FOR BUILD DIRS--
	targetdir ("build/" .. outputdir .. "/bin/%{prj.name}")
	objdir    ("build/" .. outputdir .. "/intermediates/%{prj.name}")

	-- SETTING UP nuget PACKAGES--
	nuget {'glm:0.9.9.500'}

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
		"%{prj.name}/Shader/**.vert",
		"%{prj.name}/Shader/**.frag"
	}

	includedirs
	{
		"${prj.name}/",
		"Falcon/",
		"Falcon/System",
		"%{IncludeDirs.vendor}",
		"%{IncludeDirs.glad}",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.KHR}",
		"%{IncludeDirs.boost}",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.assimp}"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FL_PLATFORM_WINDOWS"

		}

		links
		{
			"Falcon",
			"glfw3.lib",
			"opengl32.lib";
			
		}

	filter "system:linux"
		systemversion "latest"

		defines
		{
			"FL_PLATFORM_LINUX"
		}

		links 
		{
			"Falcon",
			"Xrandr",
	        "Xi",
			"glfw3",
        	"GLEW",
            "GLU",
            "GL",
            "X11",
			"dl",
			"pthread",
			"assimp"
		}
		


	filter "configurations:Debug"
		defines "FL_GAME_DEBUG"
		runtime "Debug"
		symbols "on"

		libdirs
		{
			"%{LinkDebugDirs.GLFW}",
			"%{LinkDebugDirs.boost}",
			"%{LinkDebugDirs.assimp}",
		}

		links
		{
			"assimp-vc142-mtd.lib"
		}


	filter "configurations:Release"
		defines "FL_GAME_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			"assimp-vc142-mt.lib"
		}
		libdirs
		{
			"%{LinkReleaseDirs.GLFW}",
			"%{LinkReleaseDirs.boost}",
			"%{LinkReleaseDirs.assimp}",
		}


	filter{"configurations:Debug"}
		falcon_path_deb = "build/" .. outputdir .. "/bin/Falcon"
		falcon_abs_path_deb = path.getabsolute(falcon_path_deb)
	
		postbuildcommands ('{COPY} "%{falcon_abs_path_deb}" "%{cfg.targetdir}"')

	filter{"configurations:Release"}

		falcon_path_release = "build/" .. outputdir .. "/bin/Falcon"
		falcon_abs_path_release = path.getabsolute(falcon_path_release)

		postbuildcommands ('{COPY} "%{falcon_abs_path_release}" "%{cfg.targetdir}"')