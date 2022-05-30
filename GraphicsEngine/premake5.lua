project "GraphicsEngine"
	location "%{wks.location}/%{prj.name}/"
		
	language "C++"
	cppdialect "C++20"

	targetdir ("../Lib/%{cfg.buildcfg}")
	targetname("%{prj.name}")
	objdir ("../Intermediate/%{cfg.buildcfg}/%{prj.name}")

	includedirs{
		"../Utility/",
		"../External/"
	}

	libdirs{
		"../Lib/",
		"../Lib/%{cfg.buildcfg}"
	}

	links{
		"External",
		"Utility",
		"External.lib",
		"Utility.lib",
		"d3d11.lib",
		"libfbxsdk.dll",
		"TGAFBXImporter.lib"
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
		
	local shader_dir = "../Bin/Shaders/"
    os.mkdir(shader_dir)

    filter("files:**.hlsl")
        flags("ExcludeFromBuild")
        shaderobjectfileoutput(shader_dir.."%{file.basename}"..".cso")
        shadermodel "5.0"

    filter("files:**_ps.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Pixel")
        shadermodel "5.0"

    filter("files:**_vs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Vertex")
        shadermodel "5.0"

    filter("files:**_gs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Geometry")
        shadermodel "5.0"

    filter("files:**_cs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Compute")
        shadermodel "5.0"
        filter ("files:**.c")