#include "Viewport.h"

#include <Hildur.h>

#include <ImGui/imgui.h>


#define BIND_EVENT_FN(x) std::bind(&Viewport::x, this, std::placeholders::_1)


namespace Editor {


	Viewport::Viewport()
	{

	}

	Viewport::~Viewport()
	{

	}

	void Viewport::Init(uint32_t width, uint32_t height)
	{
		m_ObjectIDFrameBuffer = Hildur::FrameBuffer::Create(width, height);
		m_ObjectIDFrameBuffer->AddTextureAttachment("ID");
		m_ObjectIDFrameBuffer->AddDepthBufferAttachment();
		m_ObjectIDFrameBuffer->Ready();
	}

	void Viewport::Render(Hildur::Ref<Hildur::FrameBuffer> framebuffer, const Hildur::Window& window)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Viewport");

		if (m_ViewportWidth != ImGui::GetWindowContentRegionMax().x || m_ViewportHeight != ImGui::GetWindowContentRegionMax().y)
		{
			m_ViewportWidth = ImGui::GetWindowContentRegionMax().x;
			m_ViewportHeight = ImGui::GetWindowContentRegionMax().y;
			m_Rezised = true;
		}
		m_ViewportPosX = ImGui::GetWindowContentRegionMin().x + ImGui::GetWindowPos().x;
		m_ViewportPosY = ImGui::GetWindowContentRegionMin().y + ImGui::GetWindowPos().y;
		m_IsInViewport = ImGui::IsWindowHovered();

		if (m_Rezised)
		{
			m_ObjectIDFrameBuffer->Resize(m_ViewportWidth, m_ViewportHeight);
			framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
			if (Hildur::Camera::GetMainCamera() != nullptr)
				Hildur::Camera::GetMainCamera()->UpdateAspect((float)m_ViewportWidth / (float)m_ViewportHeight);
			m_Rezised = false;
		}								  

		if (m_IsInViewport)
		{
			m_MouseViewportPosX = (float)((float)(Hildur::Input::GetMouseX() + window.GetPositionX() - m_ViewportPosX) / (float)m_ViewportWidth) - 0.5f;
			m_MouseViewportPosY = (float)((float)(Hildur::Input::GetMouseY() + window.GetPositionY() - m_ViewportPosY) / (float)m_ViewportHeight) - 0.5f;
		}

		ImGui::GetWindowDrawList()->AddImage(
			(void*)(intptr_t)framebuffer->GetAttachment("color")->rendererID,
			ImVec2(ImGui::GetCursorScreenPos()),
			ImVec2(ImGui::GetCursorScreenPos().x + m_ViewportWidth, ImGui::GetCursorScreenPos().y + m_ViewportHeight));

		Hildur::Renderer::OnWindowResize(m_ViewportWidth, m_ViewportHeight);

		ImGui::End();
		ImGui::PopStyleVar();

		//Test 
		ImGui::Begin("ID Buffer");
		uint32_t imageWidth = ImGui::GetWindowContentRegionMax().x;
		uint32_t imageHeight = ImGui::GetWindowContentRegionMax().y;
		ImGui::GetWindowDrawList()->AddImage(
			(void*)(intptr_t)m_ObjectIDFrameBuffer->GetAttachment("ID")->rendererID,
			ImVec2(ImGui::GetCursorScreenPos()),
			ImVec2(ImGui::GetCursorScreenPos().x + imageWidth, ImGui::GetCursorScreenPos().y + imageHeight));
		ImGui::End();
	}

	void Viewport::OnEvent(Hildur::Event& e)
	{
		Hildur::EventDispatcher	distpatcher(e);
		distpatcher.Dispatch<Hildur::WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		distpatcher.Dispatch<Hildur::WindowMoveEvent>(BIND_EVENT_FN(OnWindowMove));
		distpatcher.Dispatch<Hildur::WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		distpatcher.Dispatch<Hildur::MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseClick));
		distpatcher.Dispatch<Hildur::MouseScrolledEvent>(BIND_EVENT_FN(OnMouseScrolled));
	}

	bool Viewport::OnWindowResize(Hildur::WindowResizeEvent& e)
	{
		m_Rezised = true;

		return false;
	}

	bool Viewport::OnWindowMove(Hildur::WindowMoveEvent& e)
	{
		return false;
	}

	bool Viewport::OnWindowClose(Hildur::WindowCloseEvent& e)
	{
		return false;
	}

	bool Viewport::OnMouseClick(Hildur::MouseButtonPressedEvent& e)
	{
		if (IsMouseInViewport() && e.GetMouseButton() == 0)
		{
			glm::vec3 objectID = ReadPixel(GetMouseViewportPos().x, GetMouseViewportPos().y);
			m_SelectedEntity = Hildur::Renderer::GetEntityFromID((uint32_t)(objectID.x * 255));
			std::string name = m_SelectedEntity != nullptr ? m_SelectedEntity->m_Name : "The void!";

			HR_CORE_WARN("Click in viewport! (this is supposed to happen),  coords: ({0}, {1}), Object ID: {2}, Object name: {3}", GetMouseViewportPosNorm().x, GetMouseViewportPosNorm().y, objectID.x, name.c_str());
		}

		return false;
	}

	bool Viewport::OnMouseScrolled(Hildur::MouseScrolledEvent& e)
	{
		return false;
	}

	void Viewport::UpdateSize()
	{
	}

	glm::vec3 Viewport::ReadPixel(uint32_t x, uint32_t y)
	{
		return m_ObjectIDFrameBuffer->ReadPixel("ID", x, y);
	}


}