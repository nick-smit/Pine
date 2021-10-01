#include "pinepch.h"

#include "Application.h"
#include "Logger.h"
#include "Timestep.h"

#include "Pine\Renderer\Renderer.h"

namespace Pine {
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		s_Instance = this;
	}

	void Application::Init()
	{
		CoreLogger::Initialize();
		AppLogger::Initialize();

		PINE_LOG_CORE_INFO("Initializing PINE {0}", PINE_VERSION);

		Window::Specification windowSpec("Pine " PINE_VERSION, 720, 480);
		m_Window = std::make_shared<Window>(windowSpec);
		{
			bool success = m_Window->Initialize();
			PINE_ASSERT(success, "Failed to initialize window!");
		}

		m_GraphicsContext = GraphicsContext(m_Window);
		m_GraphicsContext.Init();
		Renderer::Init();
	}

	Application::~Application()
	{
		Renderer::Terminate();
		m_Window->Terminate();
	}

	void Application::Run()
	{
		while (!m_Window->ShouldClose()) {
			float time = m_Window->GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (auto layer : m_LayerStack) {
				layer->OnUpdate(timestep);
			}

			m_GraphicsContext.SwapBuffers();
			m_Window->PollEvents();
		}
	}

}
