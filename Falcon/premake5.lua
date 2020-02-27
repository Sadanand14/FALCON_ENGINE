workspace "Falcon"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}





outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}



IncludeDirs = {}

IncludeDirs["vendor"] = "Falcon/vendor/include/";
IncludeDirs["spdlog"] = "Falcon/vendor/include/spdlog";
IncludeDirs["GLFW"]   = "Falcon/vendor/include/glfw";
IncludeDirs["glad"]   = "Falcon/vendor/include/glad";
IncludeDirs["assimp"] = "Falcon/vendor/include/assimp";
IncludeDirs["KHR"]    = "Falcon/vendor/include/KHR";
IncludeDirs["boost"]  = "Falcon/vendor/include/boost";
IncludeDirs["physx"]  = "Falcon/vendor/include/Physx/physx/include";
IncludeDirs["physxExtensions"]= "Falcon/vendor/include/Physx/physx/source/physxextensions/src";
IncludeDirs["pxshared"]= "Falcon/vendor/include/Physx/pxshared/include";
IncludeDirs["physxSource"]="Falcon/vendor/include/Physx/physx/source/";
IncludeDirs["physxFoundation"]="Falcon/vendor/include/Physx/physx/source/foundation/";
IncludeDirs["physxFoundationIncludes"]="Falcon/vendor/include/Physx/physx/source/foundation/include";
IncludeDirs["physxGeoUtilIncludes"]="Falcon/vendor/include/Physx/physx/source/geomutils/include";
IncludeDirs["physxGeoUtilsrc"]="Falcon/vendor/include/Physx/physx/source/geomutils/src/";
IncludeDirs["physxContact"]="Falcon/vendor/include/Physx/physx/source/geomutils/src/contact";
IncludeDirs["physxCommon"]="Falcon/vendor/include/Physx/physx/source/geomutils/src/common";
IncludeDirs["physxConvex"]="Falcon/vendor/include/Physx/physx/source/geomutils/src/convex";
IncludeDirs["physxDistance"]="Falcon/vendor/include/Physx/physx/source/geomutils/src/distance ";
IncludeDirs["physxSweep"]        ="Falcon/vendor/include/Physx/physx/source/geomutils/src/sweep ";
IncludeDirs["physxGjk"]          ="Falcon/vendor/include/Physx/physx/source/geomutils/src/gjk ";
IncludeDirs["physxIntersection"] = "Falcon/vendor/include/Physx/physx/source/geomutils/src/intersection";
IncludeDirs["physxMesh"]="Falcon/vendor/include/Physx/physx/source/geomutils/src/mesh "
IncludeDirs["physxHf"]  ="Falcon/vendor/include/Physx/physx/source/geomutils/src/hf ";
IncludeDirs["physxPcm"] ="Falcon/vendor/include/Physx/physx/source/geomutils/src/pcm ";
IncludeDirs["physxCcd"] ="Falcon/vendor/include/Physx/physx/source/geomutils/src/ccd";
IncludeDirs["fmodcore"] ="Falcon/vendor/FMODStudioAPI/api/core/inc/";
IncludeDirs["fmodbank"] ="Falcon/vendor/FMODStudioAPI/api/fsbank/inc/";
IncludeDirs["fmodstudio"] ="Falcon/vendor/FMODStudioAPI/api/studio/inc/";
IncludeDirs["nuklear"] ="Falcon/vendor/include/nuklear";



LinkDebugDirs = {}
LinkDebugDirs["assimp"] = "Falcon/vendor/libs/assimp/Debug"
LinkDebugDirs["boost"]  = "Falcon/vendor/libs/boost"
LinkDebugDirs["GLFW"]   = "Falcon/vendor/libs/GLFW/Debug"
LinkDebugDirs["physx"]  = "Falcon/vendor/libs/physx/Debug"
LinkDebugDirs["fmodcore"]  = "Falcon/vendor/FMODStudioAPI/api/core/lib/x64/"
LinkDebugDirs["fmodbank"]  = "Falcon/vendor/FMODStudioAPI/api/fsbank/lib/x64/"
LinkDebugDirs["fmodstudio"]  = "Falcon/vendor/FMODStudioAPI/api/studio/lib/x64"

LinkReleaseDirs = {}
LinkReleaseDirs["assimp"] = "Falcon/vendor/libs/assimp/Release"
LinkReleaseDirs["boost"]  = "Falcon/vendor/libs/boost"
LinkReleaseDirs["GLFW"]   = "Falcon/vendor/libs/GLFW/Release"
LinkReleaseDirs["physx"]  = "Falcon/vendor/libs/physx/Release"


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
		"%{prj.name}/Physics/**.hpp",
		"%{prj.name}/Physics/**.cpp",
		"%{prj.name}/Physics/**.c",
		"%{prj.name}/Shader/**.vert",
		"%{prj.name}/Shader/**.frag",
	        "%{prj.name}/Audio/**.h",
		"%{prj.name}/Audio/**.hpp",
		"%{prj.name}/Audio/**.cpp",
	}

	includedirs
	{
		"%{prj.name}",
		"%{prj.name}/Core",
		"%{prj.name}/System",
		"%{prj.name}/Rendering",
		"%{prj.name}/Rendering/PipeLine",
        	"%{prj.name}/Audio",
        	"%{prj.name}/Physics",
		"%{IncludeDirs.vendor}",
		"%{IncludeDirs.glad}",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.KHR}",
		"%{IncludeDirs.boost}",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.assimp}",
		"%{IncludeDirs.physx}",
		"%{IncludeDirs.physxExtensions}",
		"%{IncludeDirs.pxshared}",
		"%{IncludeDirs.pxsharedFoundation}",
		"%{IncludeDirs.physxSource}",
		"%{IncludeDirs.physxFoundation}",
		"%{IncludeDirs.physxFoundationIncludes}",
		"%{IncludeDirs.physxGeoUtilIncludes}",
		"%{IncludeDirs.physxGeoUtilsrc}",
		"%{IncludeDirs.physxContact}",
		"%{IncludeDirs.physxCommon}",
		"%{IncludeDirs.physxConvex}",
		"%{IncludeDirs.physxDistance}",
		"%{IncludeDirs.physxSweep}",
		"%{IncludeDirs.physxGjk}",
		"%{IncludeDirs.physxIntersection}",
		"%{IncludeDirs.physxMesh}",
		"%{IncludeDirs.physxHf}",
		"%{IncludeDirs.physxPcm}",
		"%{IncludeDirs.physxCcd}",
		"%{IncludeDirs.fmodcore}",
		"%{IncludeDirs.fmodbank}",
		"%{IncludeDirs.fmodstudio}",
		"%{IncludeDirs.nuklear}"
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
		{       "boost_thread",
			"glfw3.lib",
			"opengl32.lib",
			"assimp-vc140-mt.lib",
			"PhysX_64.lib",
			"PhysXCommon_64.lib",
			"PhysXCooking_64.lib",
			"PhysXFoundation_64.lib",
			"PhysXPvdSDK_static_64.lib",
			"PhysXExtensions_static_64.lib",
			--"PhysXTask_static_64.lib"--
		}

		nuget {'glm:0.9.9.500'}




	filter { "system:windows", "configurations:Debug" }
			defines
			{
				"BUILD_DEBUG_MODE",
				"_DEBUG"
			}
			symbols "On"

			libdirs
			{
				"%{LinkDebugDirs.GLFW}",
				"%{LinkDebugDirs.boost}",
				"%{LinkDebugDirs.assimp}",
				"%{LinkDebugDirs.physx}",

			}


	filter { "system:windows", "configurations:Release" }

			defines
			{
				"BUILD_RELEASE_MODE",
				"_NDEBUG"
			}
			optimize "On"

			libdirs
			{
				"%{LinkReleaseDirs.GLFW}",
				"%{LinkReleaseDirs.boost}",
				"%{LinkReleaseDirs.assimp}",
				"%{LinkReleaseDirs.physx}"

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

			--Linux order for physxlibs https://github.com/NVIDIAGameWorks/PhysX/issues/92 --
			"PhysX_static_64",
			"PhysXPvdSDK_static_64",
			"PhysXVehicle_static_64",
			"PhysXExtensions_static_64",
			"PhysXCooking_static_64",
			"PhysXCommon_static_64",
			"PhysXFoundation_static_64",

			--"PhysXTask_static_64"--
		}



	filter {"system:linux","configurations:Debug"}
		defines
		{
			"BUILD_DEBUG_MODE",
			"_DEBUG"
		}
		symbols "On"

		printf("%s",LinkReleaseDirs.assimp)

		libdirs
		{
			"%{LinkDebugDirs.GLFW}",
			"%{LinkDebugDirs.boost}",
			"%{LinkDebugDirs.assimp}",
			"%{LinkDebugDirs.physx}",
			"%{LinkDebugDirs.fmodcore}",
			"%{LinkDebugDirs.fmodbank}",
			"%{LinkDebugDirs.fmodstudio}",
			"/usr/local/lib",
			"/usr/lib"
		}

		links
		{
			"fmodL",
			"fsbankL",
			"fmodstudioL"
		}


	filter {"system:linux","configurations:Release"}
		defines
		{
			"BUILD_RELEASE_MODE",
			"NDEBUG"
		}

		optimize "On"

		libdirs
		{
			"%{LinkReleaseDirs.GLFW}",
			"%{LinkReleaseDirs.boost}",
			"%{LinkReleaseDirs.assimp}",
			"%{LinkReleaseDirs.physx}",
			"%{LinkDebugDirs.fmodcore}",
			"%{LinkDebugDirs.fmodbank}",
			"%{LinkDebugDirs.fmodstudio}",
			"/usr/local/lib",
			"/usr/lib"
		}

		links
		{
			"fmod",
			"fsbank",
			"fmodstudio"
		}

	--Setting up prebuild commands--

	filter{"configurations:Debug"}
		assimp_abs_path_deb = path.getabsolute(LinkDebugDirs["assimp"])
		physx_abs_path_deb = path.getabsolute(LinkDebugDirs["physx"])
		prebuildcommands {
			'{COPY} "%{assimp_abs_path_deb}" "%{cfg.targetdir}"',
			'{COPY} "%{physx_abs_path_deb}" "%{cfg.targetdir}"',
		}

	filter{"configurations:Release"}
		assimp_abs_path_res = path.getabsolute(LinkReleaseDirs["assimp"])
		physx_abs_path_res =  path.getabsolute(LinkReleaseDirs["physx"])
		prebuildcommands {
			'{COPY} "%{assimp_abs_path_res}" "%{cfg.targetdir}"',
			'{COPY} "%{physx_abs_path_res}" "%{cfg.targetdir}"',
		}
