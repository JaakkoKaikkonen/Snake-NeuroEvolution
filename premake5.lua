workspace "SnakeNeuroEvolution"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.system}-%{cfg.buildcfg}"

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

	defines { "SFML_STATIC" }

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		links 
		{
			"sfml-graphics-s.lib",
			"sfml-window-s.lib",
			"sfml-system-s.lib",
			"sfml-main.lib",
			"opengl32.lib",
			"freetype.lib",
			"winmm.lib",
			"gdi32.lib",
			"opengl32.lib"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		links
		{
			"sfml-graphics-s-d.lib",
			"sfml-window-s-d.lib",
			"sfml-system-s-d.lib",
			"sfml-main-d.lib",
			"opengl32.lib",
			"freetype.lib",
			"winmm.lib",
			"gdi32.lib",
			"opengl32.lib"
		}
		
		
	filter "system:windows"
		cppdialect "C++17"
		systemversion "10.0.16299.0"