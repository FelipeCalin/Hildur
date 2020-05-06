#pragma once

#include <glm/glm.hpp>


namespace Hildur {


	class PerspectiveCamera
	{
	public:

		PerspectiveCamera(float fov, float aspectRatio, float nNear, float nFar);

		void SetProjection(float fov, float aspectRatio, float nNear, float nFar);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(glm::vec3 position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::vec3& GetRotation() const { return m_Rotation; }
		void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::vec3& GetForwardDir() const { return m_ForwardDir; }
		const glm::vec3& GetUpDir() const { return m_UpDir; }
		const glm::vec3& GetRightDir() const { return m_RightDir; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:

		void RecalculateViewMatrix();

	private:

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_ForwardDir = glm::vec3(0.0f);
		glm::vec3 m_UpDir = glm::vec3(0.0f);
		glm::vec3 m_RightDir = glm::vec3(0.0f);

		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Rotation = glm::vec3(0.0f);
	};


}