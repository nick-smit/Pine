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
	Pine::Texture2D::Specification spec;
	m_CheckerboardTexture = std::make_shared<Pine::Texture2D>(spec, "./assets/textures/checkerboard.png");
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

	// Render;
	Pine::RenderCommand::Clear();
	Pine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f });

	Pine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Pine::Renderer2D::QuadSpecification checkerboard;
	checkerboard.Position = { 1.0f, 1.0f, 0.0f };
	checkerboard.Scale = { 5.0f, 5.0f, 0.0f };
	checkerboard.Texture = m_CheckerboardTexture;
	checkerboard.TilingFactor = 10.0f;
	Pine::Renderer2D::DrawQuad(checkerboard);

	Pine::Renderer2D::QuadSpecification blueQuad(glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(0.5f, 0.5f, 0.0f));
	blueQuad.Color = glm::vec4(0.2f, 0.3f, 0.6f, 1.0f);
	Pine::Renderer2D::DrawQuad(blueQuad);

	Pine::Renderer2D::QuadSpecification redQuad(glm::vec3(1.5f,1.5f, 1.0f), glm::vec3(0.5f, 0.5f, 0.0f));
	redQuad.Color = glm::vec4(0.75f, 0.3f, 0.2f, 1.0f);
	Pine::Renderer2D::DrawQuad(redQuad);

	Pine::Renderer2D::QuadSpecification greenQuad(glm::vec3(1.5f, 0.5f, 1.0f), glm::vec3(0.5f, 0.75f, 0.0f), 45.0f);
	greenQuad.Color = glm::vec4(0.2f, 0.6f, 0.1f, 1.0f);
	Pine::Renderer2D::DrawQuad(greenQuad);

	Pine::Renderer2D::EndScene();
}
