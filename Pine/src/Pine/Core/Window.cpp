#include "pinepch.h"
#include "Window.h"
#include "Event.h"

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

		EventDispatcher<WindowResizeEvent>::Dispatch({ m_Spec.Width, m_Spec.Height });

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
		// Window event callbacks
		glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* window, int width, int height) {
			uint32_t w = static_cast<uint32_t>(width);
			uint32_t h = static_cast<uint32_t>(height);

			auto pine_window = reinterpret_cast<::Pine::Window*>(glfwGetWindowUserPointer(window));
			pine_window->m_Spec.Width = w;
			pine_window->m_Spec.Height = h;

			EventDispatcher<WindowResizeEvent>::Dispatch({ w, h });
		});

		glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* window) {
			EventDispatcher<WindowCloseEvent>::Dispatch({});
		});

		glfwSetWindowFocusCallback(m_WindowHandle, [](GLFWwindow* window, int in_focus) {
			if (in_focus == GLFW_FALSE) {
				EventDispatcher<WindowFocusEvent>::Dispatch({});
			}
			else {
				EventDispatcher<WindowBlurEvent>::Dispatch({});
			}
		});
		// End window event callbacks

		// Mouse event callbacks
		glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* window, int button, int action, int mods) {
			if (action == GLFW_PRESS) {
				EventDispatcher<MouseButtonPressedEvent>::Dispatch({
					static_cast<MouseButton>(button),
					static_cast<uint8_t>(mods)
				});
			}
			else {
				EventDispatcher<MouseButtonReleasedEvent>::Dispatch({
					static_cast<MouseButton>(button),
					static_cast<uint8_t>(mods)
				});
			}
		});

		glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* window, double x, double y) {
			EventDispatcher<MouseMovedEvent>::Dispatch({ (float)x, (float)y });
		});

		glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow* window, double x_offset, double y_offset) {
			EventDispatcher<MouseScrolledEvent>::Dispatch({ (float)x_offset, (float)y_offset });
		});
		// End mouse events

		// Key events
		glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* window, int key, int keycode, int action, int mods) {
			PINE_ASSERT((key != GLFW_KEY_UNKNOWN), "Unknown key was pressed");

			if (action == GLFW_PRESS) {
				EventDispatcher<KeyPressedEvent>::Dispatch({ static_cast<Key>(key), static_cast<uint8_t>(mods) });
			}
			else if (action == GLFW_RELEASE) {
				EventDispatcher<KeyReleasedEvent>::Dispatch({ static_cast<Key>(key), static_cast<uint8_t>(mods) });
			}
			else if (action == GLFW_REPEAT) {
				EventDispatcher<KeyRepeatedEvent>::Dispatch({ static_cast<Key>(key), static_cast<uint8_t>(mods) });
			}
			else {
				PINE_ASSERT(false, "Unkown action {0}", action);
			}
		});
		// End key events
	}

}
