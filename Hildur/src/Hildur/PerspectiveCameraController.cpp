#include "hrpcheaders.h"
#include "PerspectiveCameraController.h"

#include "Hildur/Core/Input.h"
#include "Hildur/util/KeyCodes.h"


namespace Hildur {


	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Fov(fov), m_ZoomLevel(2.5f), m_Camera(glm::radians(m_Fov* m_ZoomLevel * 20), aspectRatio, 0.1f, 100.0f), m_Rotation(rotation) 
	{
		HR_PROFILE_FUNCTION()

	}

	void PerspectiveCameraController::OnUpdate(Timestep ts) 
	{
		HR_PROFILE_FUNCTION()

		if (Input::IsKeyPressed(HR_KEY_A))
			m_CameraPosition -= m_Camera.GetRightDir() * m_CameraTranslationSpeed * ts.GetTimeSeconds();
		else if (Input::IsKeyPressed(HR_KEY_D))
			m_CameraPosition += m_Camera.GetRightDir() * m_CameraTranslationSpeed * ts.GetTimeSeconds();

		if (Input::IsKeyPressed(HR_KEY_W))
			m_CameraPosition -= m_Camera.GetForwardDir() * m_CameraTranslationSpeed * ts.GetTimeSeconds() * 2.0f;
		else if (Input::IsKeyPressed(HR_KEY_S))
			m_CameraPosition += m_Camera.GetForwardDir() * m_CameraTranslationSpeed * ts.GetTimeSeconds() * 2.0f;

		if (Input::IsKeyPressed(HR_KEY_SPACE))
			m_CameraPosition += m_Camera.GetUpDir() * m_CameraTranslationSpeed * ts.GetTimeSeconds() * 2.0f;
		else if (Input::IsKeyPressed(HR_KEY_LEFT_SHIFT))
			m_CameraPosition -= m_Camera.GetUpDir() * m_CameraTranslationSpeed * ts.GetTimeSeconds() * 2.0f;


		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		//m_CameraTranslationSpeed = m_ZoomLevel / 2;
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		HR_PROFILE_FUNCTION()

		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseScrolledEvent>(HR_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HR_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e) 
	{
		HR_PROFILE_FUNCTION()

		m_ZoomLevel -= e.GetYOffset() * 0.2f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(glm::radians(m_Fov * m_ZoomLevel * 20), m_AspectRatio, 0.1f, 100.0f);

		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e) 
	{
		HR_PROFILE_FUNCTION()

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(glm::radians(m_Fov * m_ZoomLevel * 20), m_AspectRatio, 0.1f, 100.0f);

		return false;
	}


}