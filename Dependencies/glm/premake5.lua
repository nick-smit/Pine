glmDir = "%{wks.location}/Pine/vendor/glm";

project "glm"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	targetdir (targetDir)
	objdir (objDir)

	files {
		"%{glmDir}/glm/**.hpp",
		"%{glmDir}/glm/**.inl",
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