workspace "SnakeNeuroEvolution"
	architecture "x64"

	configurations
	{
		--"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

project "SnakeNeuroEvolution"
	location "SnakeNeuroEvolution"
	kind "ConsoleApp"
	language "C++"

	targetdir (outputdir)
	objdir ("obj/" .. outputdir)

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"lib/SFML-2.5.0-x64/include",
		"lib/imgui-sfml-master",
		"lib/imgui-master"
	}

	libdirs
	{
		"lib/SFML-2.5.0-x64/lib"
	}


	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		links 
		{
			"sfml-graphics.lib",
			"sfml-window.lib",
			"sfml-system.lib",
			"sfml-main.lib",
			"opengl32.lib"
		}

	--[[filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		links
		{
			"sfml-graphics-d.lib",
			"sfml-window-d.lib",
			"sfml-system-d.lib",
			"sfml-main-d.lib",
			"opengl32.lib"
		}--]]
		


	filter "system:windows"
		cppdialect "C++17"
		systemversion "10.0.16299.0"