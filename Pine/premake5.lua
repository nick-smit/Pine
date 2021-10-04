project "Pine"
	kind "StaticLib"
	language "C++"
  cppdialect "C++17"
  staticruntime "on"

  pchheader "pinepch.h"
  pchsource "src/pinepch.cpp"

	targetdir (targetDir)
	objdir (objDir)

	files {
		"src/**.h",
		"src/**.cpp",
		"vendor/stb/**.h",
	}

	includedirs {
    "src",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb}",
	}

	links {
		"GLAD",
		"GLFW",
	}

	defines {
		"GLFW_INCLUDE_NONE",
		"STB_IMAGE_IMPLEMENTATION"
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