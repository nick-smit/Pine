#include <Pine.h>
#include "Pine\Core\Entrypoint.h"

Pine::Application* CreateApplication()
{
	Pine::Application* application = new Pine::Application("Sandbox App");

	return application;
}
