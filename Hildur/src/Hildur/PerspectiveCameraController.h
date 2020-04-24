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

		const glm::vec3& GetPosition() const { return m_CameraPosition; }
		void SetPosition(glm::vec3 position) { m_CameraPosition = position; }

		const glm::vec3& GetRotation() const { return m_CameraRotation; }
		void SetRotation(glm::vec3 rotation) { m_CameraRotation = rotation; }

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
		glm::vec3 m_CameraRotation = glm::vec3(0.0f);
		float m_CameraTranslationSpeed = 1.0f;
	};


}