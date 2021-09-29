#include "pinepch.h"
#include "Window.h"

namespace Pine {
	namespace Utils {
		static void ErrorCallback(int code, const char* description)
		{
			PINE_ASSERT(false, "GLFW Error: {0}", description);
		}
	}

	Window::~Window()
	{
		if (!m_Terminated)
			Terminate();
	}

	bool Window::Initialize()
	{
		PINE_ASSERT(!m_Initialized, "Window is already initialized!");

		if (!glfwInit()) {
			PINE_ASSERT(false, "Unable to initialize GLFW!");
			return false;
		}

		glfwSetErrorCallback(Utils::ErrorCallback);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_WindowHandle = glfwCreateWindow(m_Spec.Width, m_Spec.Height, m_Spec.Title.c_str(), NULL, NULL);
		if (!m_WindowHandle) {
			glfwTerminate();
			PINE_ASSERT(false, "Unable to create GLFW window!");
			return false;
		}

		glfwSetWindowUserPointer(m_WindowHandle, reinterpret_cast<void*>(this));

		RegisterEvents();

		glfwMakeContextCurrent(m_WindowHandle);

		//event_dispatcher<window_resize_event>::dispatch({ m_properties.width, m_properties.height });

		// Enable VSync
		if (m_Spec.VSync)
			glfwSwapInterval(1);

		m_Initialized = true;
		m_Terminated = false;
		return true;
	}

	void Window::Terminate()
	{
		PINE_ASSERT(m_Initialized, "Window must be initialized before terminating the window!");

		glfwDestroyWindow(m_WindowHandle);

		glfwTerminate();

		m_Terminated = true;
		m_Initialized = false;
	}

	void Window::SwapBuffers() const
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void Window::PollEvents() const
	{
		glfwPollEvents();
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_WindowHandle);
	}

	float Window::GetTime() const
	{
		return (float) glfwGetTime();
	}

	void Window::RegisterEvents()
	{
	}

}
