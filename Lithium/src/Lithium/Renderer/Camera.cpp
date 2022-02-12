#include "lipch.h"
#include "Camera.h"

#include "Lithium/Core/Application.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Li
{
	Camera::Camera()
		: m_ViewMatrix(1.0f), m_Position{ 0.0f, 0.0f, 0.0f }
	{
	}

	void Camera::SetOrtho(float left, float right, float bottom, float top)
	{
		switch (GraphicsFactory::Get()->GetAPI())
		{
		case GraphicsAPI::OpenGL:
			m_ProjectionMatrix = glm::orthoRH_NO(left, right, bottom, top, -100.0f, 100.0f);
			break;
		case GraphicsAPI::D3D11:
			m_ProjectionMatrix = glm::orthoRH_ZO(left, right, bottom, top, -100.0f, 100.0f);
			break;
		}
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::SetPerspective(float fov, float aspect, float z_near, float z_far)
	{
		switch (GraphicsFactory::Get()->GetAPI())
		{
		case GraphicsAPI::OpenGL:
			m_ProjectionMatrix = glm::perspectiveRH_NO(glm::radians(fov), aspect, z_near, z_far);
			break;
		case GraphicsAPI::D3D11:
			m_ProjectionMatrix = glm::perspectiveRH_ZO(glm::radians(fov), aspect, z_near, z_far);
			break;
		}
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::SetLookAt(const glm::vec3& camera_pos, const glm::vec3& target_pos, const glm::vec3& up)
	{
		m_Position = camera_pos;
		m_ViewMatrix = glm::lookAt(camera_pos, target_pos, up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
		m_ViewMatrix = glm::inverse(transform);
	}
}
