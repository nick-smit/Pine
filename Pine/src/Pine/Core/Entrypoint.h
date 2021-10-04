#pragma once

#include "Application.h"

extern Pine::Application* CreateApplication();

int main(int argc, char** argv)
{
	PINE_PROFILE_BEGIN_SESSION("Initialization", "./profile-init.json");
	Pine::Application* application = CreateApplication();
	PINE_PROFILE_END_SESSION();


	PINE_PROFILE_BEGIN_SESSION("Running", "./profile-run.json");
	application->Run();
	PINE_PROFILE_END_SESSION();

	PINE_PROFILE_BEGIN_SESSION("Terminate", "./profile-terminate.json");
	delete application;
	PINE_PROFILE_END_SESSION();
}
