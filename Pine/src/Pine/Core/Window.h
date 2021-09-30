#pragma once

#include "Input.h"

#include <string>

#include <GLFW\glfw3.h>

namespace Pine {

	class Window {
	public:
		struct Specification {
			std::string Title = "Untitled";
			uint32_t Width = 1280;
			uint32_t Height = 720;
			bool VSync;

			Specification(const std::string& title, uint32_t width, uint32_t height, bool vsync = true)
				: Title(title), Width(width), Height(height), VSync(vsync) {}
			Specification() = default;
		};

	public:
		Window(const Specification& spec)
			: m_Spec(spec) {};
		virtual ~Window();

	public:
		bool Initialize();
		void Terminate();

		void SwapBuffers() const;
		void PollEvents() const;

		bool ShouldClose() const;

		float GetTime() const;

		bool IsKeyPressed(Key key) const;
		bool IsMouseButtonPressed(MouseButton button) const;

		GLFWwindow* GetNativeWindow() const { return m_WindowHandle; }

	private:
		void RegisterEvents();

	private:
		Specification m_Spec;
		bool m_Initialized = false;
		bool m_Terminated = false;

		GLFWwindow* m_WindowHandle = nullptr;
	};

}
