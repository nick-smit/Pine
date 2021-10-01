#include "SandboxLayer.h"

#include <string>
#include <glm\glm.hpp>

SandboxLayer::SandboxLayer()
	: Pine::Layer("SandboxLayer")
{
	Pine::EventDispatcher<Pine::WindowResizeEvent>::Listen([](const Pine::WindowResizeEvent& e, void* data) {
		auto* cam = (Pine::OrthographicCamera*)data;
		cam->SetProjection(-(e.Width / 100.0f), e.Width / 100.0f, -(e.Height / 100.0f), e.Height / 100.0f);

		PINE_LOG_INFO("Resized camera to {0}, {1}", e.Width / 100.0f, e.Height / 100.0f);

		return false;
	}, &m_Camera);
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
