project "PineCone"
	kind "ConsoleApp"
	language "C++"
  cppdialect "C++17"
  staticruntime "On"

	targetdir (targetDir)
	objdir (objDir)

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"%{wks.location}/Pine/src",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glm}",
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
