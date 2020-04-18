#pragma once

#include "Hildur/Renderer/PerspectiveCamera.h"
#include "Hildur/Core/Timestep.h"

#include "Hildur/Events/ApplicationEvent.h"
#include "Hildur/Events/MouseEvent.h"


namespace Hildur {


	class PerspectiveCameraController
	{
	public:

		PerspectiveCameraController(float fov, float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void SetPosition(glm::vec3 pos) { m_CameraPosition = pos; }

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

	private:

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMouse(MouseMovedEvent& e);

	private:

		float m_Fov;
		float m_AspectRatio;
		float m_ZoomLevel = 0.25f;
		PerspectiveCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = glm::vec3(1.0f);
		glm::vec3 m_CameraRotation = glm::vec3(1.0f);
		float m_CameraTranslationSpeed = 1.0f;
	};


}