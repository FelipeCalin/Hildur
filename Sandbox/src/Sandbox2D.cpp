#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {



}

void Sandbox2D::OnAttach() {

	m_TextureCherno = Hildur::Texture2D::Create("assets/textures/Checkerboard.png");

}

void Sandbox2D::OnDetach() {



}

void Sandbox2D::OnUpdate(Hildur::Timestep ts) {

	// Update
	m_CameraController.OnUpdate(ts);
	//m_CameraController.SetResize(Hildur::Application::Get().GetRenderWidth(), Hildur::Application::Get().GetRenderHeight());

	// Render
	Hildur::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Hildur::RenderCommand::Clear();

	Hildur::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Hildur::Renderer2D::DrawQuad({ 0.5f, 1.0f }, { 1.0f, 1.0f }, {0.8f, 0.3f, 0.7f, 1.0f});
	Hildur::Renderer2D::DrawQuad({ 0.0f, 5.0f, -0.5f }, { 10.0f, 10.0f }, m_TextureCherno);
	Hildur::Renderer2D::DrawQuad({ 0.0f, 1.0f }, { 0.8f, 0.8f }, m_SquareColor);

	Hildur::Renderer2D::EndScene();

}

void Sandbox2D::OnImGuiRender() {

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();

}

void Sandbox2D::OnEvent(Hildur::Event& e) {

	m_CameraController.OnEvent(e);

}