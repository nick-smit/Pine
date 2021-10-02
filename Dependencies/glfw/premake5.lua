glfwDir = "%{wks.location}/Pine/vendor/glfw";

project "GLFW"
	kind "StaticLib"
	language "C"

	targetdir (targetDir)
	objdir (objDir)

	files {
		"%{glfwDir}/include/GLFW/glfw3.h",
		"%{glfwDir}/include/GLFW/glfw3native.h",
		"%{glfwDir}/src/glfw_config.h",
		"%{glfwDir}/src/context.c",
		"%{glfwDir}/src/init.c",
		"%{glfwDir}/src/input.c",
		"%{glfwDir}/src/monitor.c",
		"%{glfwDir}/src/vulkan.c",
		"%{glfwDir}/src/window.c"
	}

	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		files {
			"%{glfwDir}/src/x11_init.c",
			"%{glfwDir}/src/x11_monitor.c",
			"%{glfwDir}/src/x11_window.c",
			"%{glfwDir}/src/xkb_unicode.c",
			"%{glfwDir}/src/posix_time.c",
			"%{glfwDir}/src/posix_thread.c",
			"%{glfwDir}/src/glx_context.c",
			"%{glfwDir}/src/egl_context.c",
			"%{glfwDir}/src/osmesa_context.c",
			"%{glfwDir}/src/linux_joystick.c"
		}

		defines {
			"_GLFW_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files {
			"%{glfwDir}/src/win32_init.c",
			"%{glfwDir}/src/win32_joystick.c",
			"%{glfwDir}/src/win32_monitor.c",
			"%{glfwDir}/src/win32_time.c",
			"%{glfwDir}/src/win32_thread.c",
			"%{glfwDir}/src/win32_window.c",
			"%{glfwDir}/src/wgl_context.c",
			"%{glfwDir}/src/egl_context.c",
			"%{glfwDir}/src/osmesa_context.c"
		}

		defines { 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
