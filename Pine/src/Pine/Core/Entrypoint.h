#pragma once

#include "Application.h"

extern Pine::Application* CreateApplication();

int main(int argc, char** argv)
{
	Pine::Application* application = CreateApplication();

	application->Run();

	delete application;
}
