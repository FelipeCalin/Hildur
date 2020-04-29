#include "hrpcheaders.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Hildur {


	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nNear, float nFar)
		:m_ProjectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f)), m_ViewMatrix(1.0f)
	{
		HR_PROFILE_FUNCTION()

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

		m_Position = { 0.0f, 0.0f, 0.0f };
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nNear, float nFar) 
	{
		HR_PROFILE_FUNCTION()

		if (!isnan(fov) && !isnan(aspectRatio))
		{
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
	}

	void PerspectiveCamera::RecalculateViewMatrix() 
	{
		HR_PROFILE_FUNCTION()

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0, 1, 0)) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));


		m_ViewMatrix = glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


}