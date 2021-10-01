#pragma once

#include <Pine.h>

#include <memory>

class SandboxLayer : public Pine::Layer {
public:
	SandboxLayer()
		: Layer("SandboxLayer") {};

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Pine::Timestep ts) override;

private:
	std::shared_ptr<Pine::VertexArray> m_VertexArray = nullptr;
	std::shared_ptr<Pine::Shader> m_Shader = nullptr;
	std::shared_ptr<Pine::Texture2D> m_Texture = nullptr;
};
