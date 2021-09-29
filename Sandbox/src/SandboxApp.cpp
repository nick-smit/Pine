#include <Pine.h>
#include "Pine\Core\Entrypoint.h"

Pine::Application* CreateApplication()
{
	Pine::Application* application = new Pine::Application("Sandbox App");

	PINE_LOG_INFO("Application has been booted");

	return application;
}
