#include <Pine.h>
#include "Pine\Core\Entrypoint.h"

#include <memory>
#include "Layers\SandboxLayer.h"

Pine::Application* CreateApplication()
{
	Pine::Application* application = new Pine::Application("Sandbox App");

	application->PushLayer(std::make_shared<SandboxLayer>());

	return application;
}
