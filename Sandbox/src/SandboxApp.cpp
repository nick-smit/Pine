#include <Pine.h>
#include "Pine\Core\Entrypoint.h"

Pine::Application* CreateApplication()
{
	Pine::Application* application = new Pine::Application("Sandbox App");

	PINE_LOG_INFO("Application has been booted");

	Pine::EventDispatcher<Pine::WindowResizeEvent>::Listen([](const Pine::WindowResizeEvent& e, void* data) {
		PINE_LOG_INFO("Window resized to {0}, {1}", e.Width, e.Height);

		return false;
	});

	Pine::EventDispatcher<Pine::KeyPressedEvent>::Listen([](const Pine::KeyPressedEvent& e, void* data) {
		PINE_LOG_INFO("Key {0} was pressed", (char)e.key);

		return false;
	});

	return application;
}
