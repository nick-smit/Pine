#include "ImGuiLayer.h"

Pine::ImGuiLayer::ImGuiLayer()
	: Layer("PineCone_ImGuiLayer")
{
}

Pine::ImGuiLayer::~ImGuiLayer()
{
	for (auto fn : m_UnsubscribeFunctions) {
		fn();
	}
}

void Pine::ImGuiLayer::OnAttach()
{
	PINE_LOG_INFO("Attached ImGuiLayer");
}

void Pine::ImGuiLayer::OnDetach()
{
	PINE_LOG_INFO("Detached ImGuiLayer");
}

void Pine::ImGuiLayer::OnUpdate(Timestep ts)
{
}
