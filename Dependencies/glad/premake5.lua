gladDir = "%{wks.location}/Pine/vendor/glad";

project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "On"

	targetdir (targetDir)
	objdir (objDir)

	files {
		"%{gladDir}/include/glad/glad.h",
		"%{gladDir}/include/KHR/khrplatform.h",
		"%{gladDir}/src/glad.c"
	}

	includedirs {
		"%{gladDir}/include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"