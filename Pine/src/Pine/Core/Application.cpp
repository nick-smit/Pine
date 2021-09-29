#include "Application.h"

#include <iostream>

namespace Pine {

	Application::Application(const std::string& name)
	{
		std::cout << "Starting" << std::endl;
	}

	Application::~Application()
	{
		std::cout << "Closing" << std::endl;
	}

	void Application::Run()
	{
		std::cout << "Running" << std::endl;
	}

}