#pragma once

#include "..\Controller\OrthographicCameraController.h"

#include <Pine.h>

#include <functional>
#include <memory>
#include <vector>

class SandboxLayer : public Pine::Layer {
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Pine::Timestep ts) override;

private:
	OrthographicCameraController m_CameraController;
	std::vector<std::function<void()>> m_EventHandlers;

	std::shared_ptr<Pine::Texture2D> m_CheckerboardTexture = nullptr;
};
