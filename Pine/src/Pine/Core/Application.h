#pragma once

#include <memory>
#include "Window.h"

namespace Pine {

	class Application {
	public:
		Application(const std::string& name);
		virtual ~Application();

		void Run();

	public:
		static Application* Get() { return s_Instance; }

	private:
		bool m_Running = false;

		std::unique_ptr<Window> m_Window;

	private:
		static Application* s_Instance;
	};

}
