#include "OrthographicCameraController.h"

OrthographicCameraController::OrthographicCameraController()
{
	m_Camera.SetPosition(m_CameraPosition);
	m_Camera.SetZRotation(m_CameraRotation);

	m_EventHandlers.reserve(3);

	m_EventHandlers.push_back(Pine::EventDispatcher<Pine::WindowResizeEvent>::Listen([this](const Pine::WindowResizeEvent& e) {
		m_AspectRatio = (float)e.Width / (float)e.Height;

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}));

	m_EventHandlers.push_back(Pine::EventDispatcher<Pine::MouseScrolledEvent>::Listen([this](const Pine::MouseScrolledEvent& e) {
		m_ZoomLevel -= e.YOffset * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		float size = m_AspectRatio * m_ZoomLevel;

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}));

	m_EventHandlers.push_back(Pine::EventDispatcher<Pine::KeyPressedEvent>::Listen([this](const Pine::KeyPressedEvent& e) {
		if (e.Key == Pine::Key::R) {
			m_ZoomLevel = 1.0f;
			m_CameraPosition = glm::vec3(1.0f);
			m_CameraRotation = 0.0f;

			m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
			m_Camera.SetPosition(m_CameraPosition);
			m_Camera.SetZRotation(m_CameraRotation);
		}

		return false;
	}));
}

OrthographicCameraController::~OrthographicCameraController()
{
	for (auto unsub : m_EventHandlers) {
		unsub();
	}
}

void OrthographicCameraController::OnUpdate(Pine::Timestep ts)
{
	bool moved = false;
	if (Pine::Input::IsKeyPressed(Pine::Key::A)) {
		m_CameraPosition.x -= cos(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
		m_CameraPosition.y -= sin(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
		moved = true;
	}
	if (Pine::Input::IsKeyPressed(Pine::Key::D)) {
		m_CameraPosition.x += cos(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
		m_CameraPosition.y += sin(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
		moved = true;
	}
	if (Pine::Input::IsKeyPressed(Pine::Key::W)) {
		m_CameraPosition.x += -sin(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
		m_CameraPosition.y += cos(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
		moved = true;
	}
	if (Pine::Input::IsKeyPressed(Pine::Key::S)) {
		m_CameraPosition.x -= -sin(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
		m_CameraPosition.y -= cos(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
		moved = true;
	}

	if (moved) {
		m_Camera.SetPosition(m_CameraPosition);
	}

	bool rotated = false;
	if (Pine::Input::IsKeyPressed(Pine::Key::Q)) {
		m_CameraRotation += m_CameraRotationSpeed * ts;
		rotated = true;
	}
	if (Pine::Input::IsKeyPressed(Pine::Key::E)) {
		m_CameraRotation -= m_CameraRotationSpeed * ts;
		rotated = true;
	}

	if (rotated) {
		if (m_CameraRotation >= 180.0f) {
			m_CameraRotation -= 360.0f;
		}
		else if (m_CameraRotation < -180.0f) {
			m_CameraRotation += 360.0f;
		}

		m_Camera.SetZRotation(m_CameraRotation);
	}

}
