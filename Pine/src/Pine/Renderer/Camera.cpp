#include "pinepch.h"
#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Pine {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, zNear, zFar)), m_ViewMatrix(glm::mat4(1.0f))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetZRotation(float rotation)
	{
		m_ZRotation = rotation;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
		transform = glm::rotate(transform, glm::radians(m_ZRotation), glm::vec3(0, 0, 1));
		
		// Viewmatrix is inversed transform because when the camera moves to the left every
		// object should move to the right in the view perspective.
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
