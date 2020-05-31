#pragma once

#include <Hildur.h>


namespace Editor {


	class Viewport 
	{
	public:

		Viewport();
		~Viewport();

		void Init(uint32_t width, uint32_t height);

		void Render(Hildur::Ref<Hildur::FrameBuffer> framebuffer, const Hildur::Window& window);

		void OnEvent(Hildur::Event& e);

		bool IsMouseInViewport() const { return m_IsInViewport; }
		glm::vec2 GetMouseViewportPos() const { return m_IsInViewport ? glm::vec2((m_MouseViewportPosX + 0.5f) * m_ViewportWidth, (m_MouseViewportPosY + 0.5) * m_ViewportHeight) : glm::vec2(0.0f); }
		glm::vec2 GetMouseViewportPosNorm() const { return m_IsInViewport ? glm::vec2(m_MouseViewportPosX, m_MouseViewportPosY) : glm::vec2(0.0f); }

		Hildur::Entity* GetSelectedEntity() const { return m_SelectedEntity; }

		Hildur::Ref<Hildur::FrameBuffer> GetIDFrameBuffer() const { return m_ObjectIDFrameBuffer; }

	private:

		bool OnWindowResize(Hildur::WindowResizeEvent& e);
		bool OnWindowMove(Hildur::WindowMoveEvent& e);
		bool OnWindowClose(Hildur::WindowCloseEvent& e);

		bool OnMouseClick(Hildur::MouseButtonPressedEvent& e);
		bool OnMouseScrolled(Hildur::MouseScrolledEvent& e);

		void UpdateSize();

		glm::vec3 ReadPixel(uint32_t x, uint32_t y);

	private:

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
		uint32_t m_ViewportPosX = 0;
		uint32_t m_ViewportPosY = 0;
		float m_MouseViewportPosX = 0;
		float m_MouseViewportPosY = 0;
		bool m_IsInViewport = false;
		float m_ViewportScroll = 4;
		//glm::vec3 m_ViewportCamPos;
		//float m_ViewportCamRot;

		bool m_Rezised = true;

		Hildur::Ref<Hildur::FrameBuffer> m_ObjectIDFrameBuffer;

		Hildur::Entity* m_SelectedEntity;
	};


}