ImGuiDir = "%{wks.location}/PineCone/vendor/ImGui";

project "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir (targetDir)
	objdir (objDir)

	files {
		"%{ImGuiDir}/imconfig.h",
		"%{ImGuiDir}/imgui.h",
		"%{ImGuiDir}/imgui.cpp",
		"%{ImGuiDir}/imgui_draw.cpp",
		"%{ImGuiDir}/imgui_internal.h",
		"%{ImGuiDir}/imgui_tables.cpp",
		"%{ImGuiDir}/imgui_widgets.cpp",
		"%{ImGuiDir}/imstb_rectpack.h",
		"%{ImGuiDir}/imstb_textedit.h",
		"%{ImGuiDir}/imstb_truetype.h",
		"%{ImGuiDir}/imgui_demo.cpp",
		"%{ImGuiDir}/backends/imgui_impl_glfw.h",
		"%{ImGuiDir}/backends/imgui_impl_glfw.cpp",
		"%{ImGuiDir}/backends/imgui_impl_opengl3.h",
		"%{ImGuiDir}/backends/imgui_impl_opengl3.cpp",
	}

	includedirs {
		"%{ImGuiDir}",
		"%{IncludeDir.glfw}",
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