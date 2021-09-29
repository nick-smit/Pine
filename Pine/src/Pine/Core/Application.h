#pragma once

#include <memory>
#include "Window.h"
#include "Input.h"
#include "LayerStack.h"

namespace Pine {

	class Application {
	public:
		Application(const std::string& name);
		virtual ~Application();

		void Run();

		LayerStack GetLayerStack() const { return m_LayerStack; }

	public:
		static Application* Get() { return s_Instance; }

	private:
		bool m_Running = false;

		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;

		friend class Input;
	};

}
