#pragma once

#include <Pine.h>

#include <glm\glm.hpp>

class OrthographicCameraController {
public:
	OrthographicCameraController();
	virtual ~OrthographicCameraController();

	void OnUpdate(Pine::Timestep ts);

	const Pine::OrthographicCamera& GetCamera() const { return m_Camera; }
private:
	Pine::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition = glm::vec3(1.0f);
	float m_CameraTranslationSpeed = 1.0f;
	float m_CameraRotationSpeed = 60.0f;
	float m_CameraRotation = 0.0f;

	float m_ZoomLevel = 1.0f;
	float m_AspectRatio = 16.0f / 9.0f;

	std::vector<std::function<void()>> m_EventHandlers;
};