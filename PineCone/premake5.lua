project "PineCone"
	kind "ConsoleApp"
	language "C++"
  cppdialect "C++17"
  staticruntime "on"

  pchheader "pcpch.h"
  pchsource "src/pcpch.cpp"

	targetdir (targetDir)
	objdir (objDir)

	files {
		"src/**.h",
		"src/**.cpp",
	}

	includedirs {
		"src",
		"%{wks.location}/Pine/src",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.spdlog}",
	}

	links {
		"Pine",
		"ImGui",
		"ImGuizmo",
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
