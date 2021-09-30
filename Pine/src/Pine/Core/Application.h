#pragma once

#include "Core.h"
#include "Window.h"
#include "Input.h"
#include "LayerStack.h"

#include <memory>

namespace Pine {

	class Application {
	public:
		Application(const std::string& name);
		virtual ~Application();

		void Run();


		void PushLayer(std::shared_ptr<Layer> layer) { m_LayerStack.PushLayer(layer); };
		void PopLayer(std::shared_ptr<Layer> layer) { m_LayerStack.PopLayer(layer); };

		void PushOverlay(std::shared_ptr<Layer> layer) { m_LayerStack.PushOverlay(layer); };
		void PopOverlay(std::shared_ptr<Layer> layer) { m_LayerStack.PopOverlay(layer); };

	public:
		static Application* Get() { return s_Instance; }

	private:
		bool m_Running = false;

		float m_LastFrameTime = 0.0f;

		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;

		friend class Input;
	};

}
