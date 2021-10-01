#include "OrthographicCameraController.h"

OrthographicCameraController::OrthographicCameraController()
{
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

			m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
			m_Camera.SetPosition(m_CameraPosition);
			m_Camera.SetZRotation(0.0f);
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
	if (Pine::Input::IsKeyPressed(Pine::Key::A)) {
		m_CameraPosition.x -= cos(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
		m_CameraPosition.y -= sin(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
	}
	if (Pine::Input::IsKeyPressed(Pine::Key::D)) {
		m_CameraPosition.x += cos(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
		m_CameraPosition.y += sin(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
	}
	if (Pine::Input::IsKeyPressed(Pine::Key::W)) {
		m_CameraPosition.x += -sin(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
		m_CameraPosition.y += cos(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
	}
	if (Pine::Input::IsKeyPressed(Pine::Key::S)) {
		m_CameraPosition.x -= -sin(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
		m_CameraPosition.y -= cos(glm::radians(m_Camera.GetZRotation())) * m_CameraTranslationSpeed * ts;
	}

	if (Pine::Input::IsKeyPressed(Pine::Key::Q)) {
		float rotation = m_Camera.GetZRotation();
		rotation += m_CameraRotationSpeed * ts;
		m_Camera.SetZRotation(rotation);
	}
	if (Pine::Input::IsKeyPressed(Pine::Key::E)) {
		float rotation = m_Camera.GetZRotation();
		rotation -= m_CameraRotationSpeed * ts;
		m_Camera.SetZRotation(rotation);
	}

	m_Camera.SetPosition(m_CameraPosition);
}
