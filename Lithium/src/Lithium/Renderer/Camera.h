#pragma once

#include "glm/glm.hpp"

namespace Li 
{
	class Camera
	{
	public:
		Camera();

		void SetOrtho(float left, float right, float bottom, float top);
		void SetPerspective(float fov, float aspect, float z_near, float z_far);
		void SetPosition(const glm::vec3& position);
		void SetLookAt(const glm::vec3& camera_pos, const glm::vec3& target_pos, const glm::vec3& up);

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;
	};
}
