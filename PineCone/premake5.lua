project "PineCone"
	kind "ConsoleApp"
	language "C++"
  cppdialect "C++17"
  staticruntime "on"

	targetdir (targetDir)
	objdir (objDir)

	files {
		"src/**.h",
		"src/**.cpp",
	}

	includedirs {
		"src",
		"%{wks.location}/Pine/src",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glm}",
    "%{IncludeDir.ImGui}",
		"%{IncludeDir.spdlog}",
	}

	links {
		"Pine",
    "ImGui",
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
