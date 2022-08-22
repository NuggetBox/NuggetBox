project "GraphicsEngine"
	location "%{wks.location}/Source/%{prj.name}/"
		
	language "C++"
	cppdialect "C++20"

	targetdir ("../../Lib/%{cfg.buildcfg}")
	targetname("%{prj.name}")
	objdir ("../../Intermediate/%{cfg.buildcfg}/%{prj.name}")

	pchheader "NuggetBox.pch.h"
	pchsource "NuggetBox.pch.cpp"

	includedirs{
		"../Utility/",
		"../../External/"
	}

	libdirs{
		"../../Lib/",
		"../../Lib/%{cfg.buildcfg}"
	}

	links{
		"External",
		"Utility",
		"External.lib",
		"Utility.lib",
		"d3d11.lib",
		"libfbxsdk.dll"
	}

	files {
		"**.h",
		"**.cpp",
		"**.hpp",
		"**.hlsl",
		"**.hlsli"
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
		kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"
		flags { 
			"MultiProcessorCompile"
		}
		
	local shader_dir = "../../Bin/Shaders/"
    os.mkdir(shader_dir)

	shadermodel "5.0"

    filter("files:**.hlsl")
		flags("ExcludeFromBuild")
		shaderobjectfileoutput(shader_dir.."%{file.basename}"..".cso")
		
		filter "configurations:Debug"
			shaderdefines {"_DEBUG"}
		filter "configurations:Release"
			shaderdefines {"_RELEASE"}

    filter("files:**_ps.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Pixel")

    filter("files:**_vs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Vertex")

    filter("files:**_gs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Geometry")

    filter("files:**_cs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Compute")
        filter ("files:**.c")