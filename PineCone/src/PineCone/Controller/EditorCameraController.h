#pragma once

#include <Pine.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Pine {

	class EditorCameraController
	{
	public:
		EditorCameraController();
		virtual ~EditorCameraController();

		void Initialize();
		void Reset();

		void Focus(const glm::vec3& focusPoint);

		void SetCameraProjection(float fieldOfView, const glm::vec2& viewportSize, float zNear, float zFar);

		void Update(Timestep ts);
		void ResizeViewport(const glm::vec2& size);

		float GetFieldOfView() const { return m_FieldOfView; }
		void SetFieldOfView(float fieldOfView);

		float GetZNear() const { return m_ZNear; }
		void SetZNear(float zNear);

		float GetZFar() const { return m_ZFar; }
		void SetZFar(float zFar);

		const PerspectiveCamera& GetCamera() const { return m_Camera; }

	private:
		void RecalculateCameraProjection();
		void UpdateView();

		void Pan(const glm::vec2& delta);
		std::pair<float, float> PanSpeed() const;

		void Rotate(const glm::vec2& delta);
		float RotationSpeed() const;

		void Zoom(float offset);
		float ZoomSpeed() const;

		glm::vec3 GetForwardDirection() const;
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetRotation() const;
		glm::quat GetOrientation() const;
	private:
		float m_FieldOfView = 45.0f;
		float m_AspectRatio = 1.667f;
		float m_ZNear = 0.1f;
		float m_ZFar = 1000.0f;

		glm::vec2 m_ViewportSize;

		PerspectiveCamera m_Camera;

		float m_Distance = 25.0f;
		glm::vec3 m_FocalPoint = glm::vec3(0.0f, 0.0f, m_Distance);
		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;

		glm::vec2 m_MousePosition = { 0.0f, 0.0f };

		std::vector<std::function<void()>> m_EventUnsubscribers;
	};

}
