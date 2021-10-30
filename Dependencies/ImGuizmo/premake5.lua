ImGuizmoDir = "%{wks.location}/PineCone/vendor/ImGuizmo";

project "ImGuizmo"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir (targetDir)
	objdir (objDir)

	files {
		"%{ImGuizmoDir}/ImGuizmo.h",
		"%{ImGuizmoDir}/ImGuizmo.cpp",
	}

	includedirs {
		"%{ImGuizmoDir}",
		"%{IncludeDir.ImGui}",
	}

	filter "system:linux"
		pic "On"
		systemversion "latest"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"