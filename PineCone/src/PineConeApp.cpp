#include "Layers\ImGuiLayer.h"

#include <Pine.h>
#include "Pine\Core\Entrypoint.h"

#include <memory>

using namespace Pine;

Application* CreateApplication()
{
	Application* application = new Application("PineCone");

	application->PushLayer(std::make_shared<ImGuiLayer>());

	return application;
}