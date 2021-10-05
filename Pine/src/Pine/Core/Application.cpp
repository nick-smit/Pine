#include "pinepch.h"

#include "Application.h"
#include "Event.h"
#include "Logger.h"
#include "Timestep.h"

#include "Pine\Renderer\Renderer.h"

namespace Pine {
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		PINE_PROFILE_FUNCTION();

		s_Instance = this;

		CoreLogger::Initialize();
		AppLogger::Initialize();

		PINE_LOG_CORE_INFO("Initializing PINE {0}", PINE_VERSION);
		

		std::string pineVersion = " (Pine " PINE_VERSION ")";
		pineVersion.insert(0, name);

		Window::Specification windowSpec(pineVersion, 1600, 900);
		m_Window = std::make_shared<Window>(windowSpec);
		{
			bool success = m_Window->Initialize();
			PINE_ASSERT(success, "Failed to initialize window!");
		}

		m_GraphicsContext = GraphicsContext(m_Window);
		m_GraphicsContext.Init();
		Renderer::Init();

		EventDispatcher<WindowCloseEvent>::Listen([this](const WindowCloseEvent& e) {
			m_Running = false;

			return false;
		});
	}

	Application::~Application()
	{
		PINE_PROFILE_FUNCTION();

		m_LayerStack.Terminate();

		Renderer::Terminate();
		m_Window->Terminate();
	}

	void Application::Run()
	{
		PINE_PROFILE_FUNCTION();

		while (m_Running) {
			PINE_PROFILE_SCOPE("Pine::Application::Run - Run Loop");

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

	void Application::Close()
	{
		m_Running = false;
	}

}
