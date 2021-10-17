#include "pcpch.h"
#include "EditorCameraController.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Pine {

	EditorCameraController::EditorCameraController()
		: m_Camera(m_FieldOfView, m_AspectRatio, m_ZNear, m_ZFar)
	{
		Reset();
		UpdateView();
	}

	EditorCameraController::~EditorCameraController()
	{
		for (auto unsub : m_EventUnsubscribers) {
			unsub();
		}
	}

	void EditorCameraController::Initialize()
	{
		m_EventUnsubscribers.push_back(EventDispatcher<MouseScrolledEvent>::Listen([&](const MouseScrolledEvent& e) {
			if (e.YOffset != 0.0f) {
				Zoom(e.YOffset * 0.1f);
				UpdateView();
			}

			return false;
		}));
	}

	void EditorCameraController::Reset()
	{
		m_Distance = 100.0f;
		m_FocalPoint = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Yaw = 0.0f;
		m_Pitch = glm::radians(45.0f);

		UpdateView();
	}

	void EditorCameraController::Focus(const glm::vec3& focusPoint)
	{
		m_Distance = 50.0f;
		m_FocalPoint = glm::vec3(focusPoint.x, focusPoint.y, focusPoint.z);
		m_Yaw = 0.0f;
		m_Pitch = glm::radians(45.0f);

		UpdateView();
	}

	void EditorCameraController::SetCameraProjection(float fieldOfView, const glm::vec2& viewportSize, float zNear, float zFar)
	{
		m_FieldOfView = fieldOfView;
		m_AspectRatio = viewportSize.x / viewportSize.y;
		m_ZNear = zNear;
		m_ZFar = zFar;

		RecalculateCameraProjection();
	}

	void EditorCameraController::Update(Timestep ts)
	{
		bool shouldUpdateView = false;
		if (Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt)) {
			const glm::vec2& mouse = { Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_MousePosition) * 0.003f;
			m_MousePosition = mouse;

			if (Input::IsMouseButtonPressed(MouseButton::Right)) {
				Zoom(delta.y);
				shouldUpdateView = true;
			}
			else if (Input::IsMouseButtonPressed(MouseButton::Middle)) {
				Pan(delta);
				shouldUpdateView = true;
			}
			else if (Input::IsMouseButtonPressed(MouseButton::Left)) {
				Rotate(delta);
				shouldUpdateView = true;
			}
		}
		
		if (shouldUpdateView) {
			UpdateView();
		}
	}

	void EditorCameraController::ResizeViewport(const glm::vec2& size)
	{
		m_ViewportSize = size;
		m_AspectRatio = size.x / size.y;
		RecalculateCameraProjection();
	}

	void EditorCameraController::SetFieldOfView(float fieldOfView)
	{
		m_FieldOfView = fieldOfView;
		RecalculateCameraProjection();
	}

	void EditorCameraController::SetZNear(float zNear)
	{
		m_ZNear = zNear;
		RecalculateCameraProjection();
	}

	void EditorCameraController::SetZFar(float zFar)
	{
		m_ZFar = zFar;
		RecalculateCameraProjection();
	}

	void EditorCameraController::SetFocalPoint(const glm::vec3& focalPoint)
	{
		m_FocalPoint = focalPoint;
		UpdateView();
	}

	void EditorCameraController::SetYaw(float yaw)
	{
		m_Yaw = glm::radians(yaw);
		UpdateView();
	}

	void EditorCameraController::SetPitch(float pitch)
	{
		m_Pitch = glm::radians(pitch);
		UpdateView();
	}

	void EditorCameraController::SetDistance(float distance)
	{
		m_Distance = distance;
		UpdateView();
	}

	void EditorCameraController::RecalculateCameraProjection()
	{
		m_Camera.SetProjection(glm::radians(m_FieldOfView), m_AspectRatio, m_ZNear, m_ZFar);
	}

	void EditorCameraController::UpdateView()
	{
		m_Camera.UpdateView(m_FocalPoint - GetForwardDirection() * m_Distance, GetRotation());
	}

	void EditorCameraController::Pan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += GetRightDirection() * delta.x * xSpeed * (m_Distance * 0.1f);
		m_FocalPoint -= GetUpDirection() * delta.y * ySpeed * (m_Distance * 0.1f);
	}

	std::pair<float, float> EditorCameraController::PanSpeed() const
	{
		// Calculate panning speed based on the size of the screen
		float x = std::min(m_ViewportSize.x / 1000.0f, 2.4f);
		float xFactor = 0.0366f * (x * x) * x + 0.3021f;

		float y = std::min(m_ViewportSize.y / 1000.0f, 2.4f);
		float yFactor = 0.0366f * (y * y) * y + 0.3021f;

		return { xFactor, yFactor };
	}

	void EditorCameraController::Rotate(const glm::vec2& delta)
	{
		constexpr float maxRadians = glm::radians(360.0f);

		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		if (m_Yaw > maxRadians) {
			m_Yaw -= maxRadians;
		}
		else if (m_Yaw < 0.0f) {
			m_Yaw += maxRadians;
		}

		m_Pitch += delta.y * RotationSpeed();
		if (m_Pitch > maxRadians) {
			m_Pitch -= maxRadians;
		}
		else if (m_Pitch < 0.0f) {
			m_Pitch += maxRadians;
		}
	}

	float EditorCameraController::RotationSpeed() const
	{
		return 0.8f;
	}

	void EditorCameraController::Zoom(float offset)
	{
		m_Distance -= offset * ZoomSpeed();

		// Zooming too close to the focal point makes the zoomspeed too low
		// So we move the focal point forward, as this mimics zooming in further
		if (m_Distance < 1.0f) {
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	float EditorCameraController::ZoomSpeed() const
	{
		// Calculate zoomspeed based on distance to focal point.
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);

		return speed;
	}

	glm::vec3 EditorCameraController::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCameraController::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCameraController::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCameraController::GetRotation() const
	{
		return glm::vec3(-m_Pitch, -m_Yaw, 0.0f);
	}

	glm::quat EditorCameraController::GetOrientation() const
	{
		return glm::quat(GetRotation());
	}

}