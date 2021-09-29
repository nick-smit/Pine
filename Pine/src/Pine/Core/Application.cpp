#include "pinepch.h"
#include "Application.h"
#include "Logger.h"

namespace Pine {
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		s_Instance = this;

		CoreLogger::Initialize();
		AppLogger::Initialize();

		PINE_LOG_CORE_INFO("Initializing PINE {0}", PINE_VERSION);

		Window::Specification windowSpec("Pine " PINE_VERSION, 720, 480);
		m_Window = std::make_unique<Window>(windowSpec);
		{
			bool success = m_Window->Initialize();
			PINE_ASSERT(success, "Failed to initialize window!");
		}
	}

	Application::~Application()
	{
		m_Window->Terminate();
	}

	void Application::Run()
	{
		while (!m_Window->ShouldClose()) {
			for (auto layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_Window->SwapBuffers();
			m_Window->PollEvents();
		}
	}

}
