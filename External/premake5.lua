project "External"
	location "%{wks.location}/External/"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	targetdir ("../Lib/%{cfg.buildcfg}")
	targetname("%{prj.name}")
	objdir ("../Intermediate/%{cfg.buildcfg}/%{prj.name}")

	includedirs{
		"imgui/",
		"freetype/include/",
		"TGAFBXImporter/include/"
	}

	files {
		"**.h",
		"**.cpp",
		"**.hpp"
	}
	

	libdirs{
		"TGAFBXImporter/lib/%{cfg.buildcfg}"
	}

	links{
		"TGAFBXImporter.lib"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "system:windows"
		kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"
		flags { 
			"MultiProcessorCompile"
		}