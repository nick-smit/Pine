#include "pinepch.h"
#include "Application.h"
#include "Logger.h"

namespace Pine {

	Application::Application(const std::string& name)
	{
		CoreLogger::Initialize();
		AppLogger::Initialize();
	}

	Application::~Application()
	{
		std::cout << "Closing" << std::endl;
	}

	void Application::Run()
	{
		PINE_LOG_CORE_INFO("Application is running!");
	}

}
