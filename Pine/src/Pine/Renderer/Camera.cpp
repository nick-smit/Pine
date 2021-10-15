#include "pinepch.h"
#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Pine {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, zNear, zFar)), m_ViewMatrix(1.0f)
	{
		PINE_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		PINE_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		PINE_PROFILE_FUNCTION();

		m_Position = position;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetZRotation(float rotation)
	{
		PINE_PROFILE_FUNCTION();

		m_ZRotation = rotation;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		PINE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_ZRotation), glm::vec3(0, 0, 1));
		
		// Viewmatrix is inversed transform because when the camera moves to the left every
		// object should move to the right from the cameras perspective.
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	PerspectiveCamera::PerspectiveCamera(float fieldOfView, float aspectRatio, float zNear, float zFar)
	{
		PINE_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetProjection(float fieldOfView, float aspectRatio, float zNear, float zFar)
	{
		PINE_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetPosition(const glm::vec3& position)
	{
		PINE_PROFILE_FUNCTION();

		m_Position = position;
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetRotation(const glm::vec3& rotation)
	{
		PINE_PROFILE_FUNCTION();

		m_Rotation = rotation;
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::UpdateView(const glm::vec3& position, const glm::vec3& rotation)
	{
		PINE_PROFILE_FUNCTION();

		m_Position = position;
		m_Rotation = rotation;
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		PINE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(glm::quat(m_Rotation));

		// Viewmatrix is inversed transform because when the camera moves to the left every
		// object should move to the right from the cameras perspective.
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
