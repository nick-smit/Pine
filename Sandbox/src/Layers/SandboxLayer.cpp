#include "SandboxLayer.h"

#include <string>
#include <glm\glm.hpp>

SandboxLayer::SandboxLayer()
	: Pine::Layer("SandboxLayer")
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
}

void SandboxLayer::OnDetach()
{
	for (auto unsub : m_EventHandlers) {
		unsub();
	}
}

void SandboxLayer::OnUpdate(Pine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Pine::RenderCommand::ClearColorBufferBit();
	Pine::RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.4f });

	Pine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Pine::Renderer2D::DrawQuad(glm::vec3(0.5f, 1.0f, 1.0f), glm::vec2(1.0f), glm::vec4(1.0f));
	Pine::Renderer2D::DrawQuad(glm::vec3(1.5f, 1.5f, 1.0f), glm::vec2(0.75f), glm::vec4(0.4f, 0.5f, 0.2f, 1.0f));
	Pine::Renderer2D::DrawQuad(glm::vec3(1.5f, 0.5f, 1.0f), glm::vec2(0.5f, 0.25f), glm::vec4(0.75f, 0.2f, 0.3f, 1.0f));
	Pine::Renderer2D::EndScene();
}
