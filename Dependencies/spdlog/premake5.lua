spdlogDir = "%{wks.location}/Pine/vendor/spdlog";

project "spdlog"
	kind "StaticLib"
	language "C++"

	targetdir (targetDir)
	objdir (objDir)

	files {
		"%{spdlogDir}/include/spdlog/**.h",
	}

	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"