yamlCppDir = "%{wks.location}/Pine/vendor/yaml-cpp";

project "yaml-cpp"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	targetdir (targetDir)
	objdir (objDir)

	files {
		"%{yamlCppDir}/src/**.h",
		"%{yamlCppDir}/src/**.cpp",
		"%{yamlCppDir}/include/**.h"
	}

	includedirs {
		"%{yamlCppDir}/include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"