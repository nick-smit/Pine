#include "SandboxLayer.h"

void SandboxLayer::OnAttach()
{
	PINE_LOG_INFO("Press any key with while holding shift to print the pressed key");

	Pine::EventDispatcher<Pine::WindowResizeEvent>::Listen([](const Pine::WindowResizeEvent& e, void* data) {
		PINE_LOG_INFO("Window resized to {0}, {1}", e.Width, e.Height);

		return false;
	});

	Pine::EventDispatcher<Pine::KeyPressedEvent>::Listen([](const Pine::KeyPressedEvent& e, void* data) {
		if (e.key != Pine::Key::LeftShift && Pine::Input::isKeyPressed(Pine::Key::LeftShift)) {
			PINE_LOG_INFO("Key {0} was pressed", (char)e.key);
		}

		return false;
	});
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate()
{
}
