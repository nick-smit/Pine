include "dependencies/dependencies.lua"

workspace "Pine"
  architecture "x64"
  
  configurations {
    "Debug",
    "Release",
    "Distribution"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "dependencies/spdlog"

group ""
project "Pine"
	location "Pine"
	kind "StaticLib"
	language "C++"
  cppdialect "C++17"
  staticruntime "on"

  pchheader "pinepch.h"
  pchsource "%{prj.name}/src/pinepch.cpp"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
    "%{prj.name}/src",
		"%{IncludeDir.spdlog}",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "PINE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PINE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PINE_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
  cppdialect "C++17"
  staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Pine/src",
		"%{IncludeDir.spdlog}",
	}

	links {
		"Pine"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "PINE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PINE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PINE_DIST"
		optimize "On" 
