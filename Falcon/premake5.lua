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
IncludeDirs["ozz"]    = "Falcon/vendor/include/ozz";


LinkDebugDirs = {}
LinkDebugDirs["assimp"] = "Falcon/vendor/libs/assimp/Debug"
LinkDebugDirs["boost"]  = "Falcon/vendor/libs/boost"
LinkDebugDirs["GLFW"]   = "Falcon/vendor/libs/GLFW/Debug"
LinkDebugDirs["ozz"]    = "Falcon/vendor/libs/ozz/Debug"

LinkReleaseDirs = {}
LinkReleaseDirs["assimp"] = "Falcon/vendor/libs/assimp/Release"
LinkReleaseDirs["boost"]  = "Falcon/vendor/libs/boost"
LinkReleaseDirs["GLFW"]   = "Falcon/vendor/libs/GLFW/Release"
LinkReleaseDirs["ozz"]    = "Falcon/vendor/libs/ozz/Release"



printf("%s",LinkReleaseDirs.assimp)


project "Falcon"
	location "Falcon"
	kind "ConsoleApp"
	language "C++"


	-- SETTING UP PATHS FOR BUILD DIRS--
	targetdir ("build/" .. outputdir .. "/bin/%{prj.name}")
	objdir    ("build/" .. outputdir .. "/intermediates/%{prj.name}")


	--SETTING UP BASIC FILES CONTENT AND INCLUDE DIRS--
	files
	{
		"%{prj.name}/Core/**.h",
		"%{prj.name}/Core/**.hpp",
		"%{prj.name}/Core/**.cpp",
		"%{prj.name}/Core/**.c",
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
		"%{prj.name}/Core",
		"%{prj.name}/System",
		"%{prj.name}/Rendering",
		"%{prj.name}/Rendering/PipeLine",
		"%{IncludeDirs.vendor}",
		"%{IncludeDirs.glad}",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.KHR}",
		"%{IncludeDirs.boost}",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.assimp}",
		"%{IncludeDirs.ozz}"
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
			"opengl32.lib",
			"assimp-vc140-mt.lib",
			"ozz_animation.lib",
			"ozz_base.lib",
			"ozz_geometry.lib",
			"ozz_options.lib"
		}

		nuget {'glm:0.9.9.500'}




	filter { "system:windows", "configurations:Debug" }
			defines "BUILD_DEBUG_MODE"
			symbols "On"

			libdirs
			{
				"%{LinkDebugDirs.GLFW}",
				"%{LinkDebugDirs.boost}",
				"%{LinkDebugDirs.assimp}",
				"%{LinkDebugDirs.ozz}"
			}



	filter { "system:windows", "configurations:Release" }

			defines "BUILD_RELEASE_MODE"
			optimize "On"

			libdirs
			{
				"%{LinkReleaseDirs.GLFW}",
				"%{LinkReleaseDirs.boost}",
				"%{LinkReleaseDirs.assimp}",
				"%{LinkReleaseDirs.ozz}"
			}


	--------SETTING UP THINGS FOR LINUX

	filter "system:linux"
		pic "On"
		cppdialect "C++17"
		systemversion "latest"
		runpathdirs { "%{cfg.targetdir}" }

		defines
		{
			"FL_PLATFORM_UNIX"
		}

		links
		{

			"boost_thread",
			"Xrandr",
	        "Xi",
			"glfw3",
        	"GLEW",
            "GLU",
            "GL",
            "X11",
			"dl",
			"pthread",
			"assimp",
			"ozz_animation",
			"ozz_base",
			"ozz_geometry",
			"ozz_options"
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
			"%{LinkDebugDirs.ozz}",
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
			"%{LinkReleaseDirs.ozz}",
			"/usr/local/lib",
			"/usr/lib"
		}


	--Setting up prebuild commands--

	filter{"configurations:Debug"}
		assimp_abs_path_deb = path.getabsolute(LinkDebugDirs["assimp"])

		prebuildcommands ('{COPY} "%{assimp_abs_path_deb}" "%{cfg.targetdir}"')

	filter{"configurations:Release"}
		assimp_abs_path_res = path.getabsolute(LinkReleaseDirs["assimp"])

		prebuildcommands ('{COPY} "%{assimp_abs_path_res}" "%{cfg.targetdir}"')
