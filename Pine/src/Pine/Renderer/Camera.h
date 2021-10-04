#pragma once

#include <glm\glm.hpp>

namespace Pine {

	class Camera {
	public:
		Camera() = default;
		Camera(const Camera& other) = delete;
		virtual ~Camera() = default;

		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	protected:
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
	};

	class OrthographicCamera : public Camera {
	public:
		OrthographicCamera() = default;

		OrthographicCamera(float left, float right, float bottom, float top, float zNear = -1000.0f, float zFar = 1000.0f);
		virtual ~OrthographicCamera() = default;

		void SetProjection(float left, float right, float bottom, float top, float zNear = -1000.0f, float zFar = 1000.0f);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position);
		
		float GetZRotation() const { return m_ZRotation; };
		void SetZRotation(float rotation);
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

		glm::vec3 m_Position = glm::vec3(0.0f);
		float m_ZRotation = 0.0f;
	};

}
