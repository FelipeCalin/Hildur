#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVA = Hildur::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Hildur::Ref<Hildur::VertexBuffer> squareVB;
	squareVB = Hildur::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ Hildur::ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Hildur::Ref<Hildur::IndexBuffer> squareIB;
	squareIB = Hildur::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = Hildur::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hildur::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);
	m_CameraController.SetResize(Hildur::Application::Get().GetRenderWidth(), Hildur::Application::Get().GetRenderHeight());

	// Render
	Hildur::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Hildur::RenderCommand::Clear();

	Hildur::Renderer::BeginScene(m_CameraController.GetCamera());

	// TODO: Add these functions: - Shader::SetMat4 Shader::SetFloat4
	std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Hildur::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Hildur::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hildur::Event& e)
{
	m_CameraController.OnEvent(e);
}