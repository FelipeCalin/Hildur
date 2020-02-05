#include "hrpcheaders.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Hildur {


	Camera::Camera(const glm::mat4& projectionMatrix = glm::perspective(1.57f, 16.0f / 9.0f, 0.1f, 100.0f))
		:m_ProjectionMatrix(projectionMatrix) {

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}

	void Camera::RecalculateViewMatrix() {

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		
		//OrthoGraphic
		//m_ViewMatrix = glm::inverse(transform);

		//Perspective
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}


}