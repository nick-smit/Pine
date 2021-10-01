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
}

void SandboxLayer::OnUpdate(Pine::Timestep ts)
{
	Pine::RenderCommand::ClearColorBufferBit();
	Pine::RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.4f });

	Pine::Renderer2D::BeginScene(m_Camera);
	Pine::Renderer2D::DrawQuad({ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	Pine::Renderer2D::EndScene();
}
