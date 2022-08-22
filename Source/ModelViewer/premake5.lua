project "ModelViewer"
	location "%{wks.location}/Source/ModelViewer/"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"

	targetdir ("../../Bin/")
	targetname("%{prj.name}%{cfg.buildcfg}")
	objdir ("../../Intermediate/%{cfg.buildcfg}/%{prj.name}")

	debugdir "../../Bin/"

	files {
		"**.h",
		"**.cpp",
		"**.hpp"
	}

	includedirs{
		"../GraphicsEngine/",
		"../../External/",
		"../Utility/"
	}

	libdirs{ 
		"../../Lib/",
		"../../Lib/%{cfg.buildcfg}"
	}

	links{
		"GraphicsEngine",
		"External"
	}

	filter "configurations:Debug"
		runtime "Debug"
		defines {"_DEBUG"}
		symbols "on"
	filter "configurations:Release"
		runtime "Release"
		defines {"_RELEASE"}
		optimize "on"

	filter "system:windows"
		symbols "On"		
		systemversion "latest"
		flags { 
			"MultiProcessorCompile"
		}
		defines {
			"WIN32",
			"_LIB"
		}