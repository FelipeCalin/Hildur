#include "Sandbox3D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox3D::Sandbox3D()
	:Layer("Sandbox3D"), m_CameraController(45.0f, 16.0f / 9.0f, true), m_ObjectRot(0.0f, 0.0f, 0.0f)
{
	//Shader

	Hildur::Ref<Hildur::Shader> m_TextureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	Hildur::Ref<Hildur::Shader> m_PhongShader = m_ShaderLibrary.Load("assets/shaders/Phong.glsl");
	Hildur::Ref<Hildur::Shader> m_LightShader = m_ShaderLibrary.Load("assets/shaders/Light.glsl");
	Hildur::Ref<Hildur::Shader> m_FlatColorShader = m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");

	Hildur::Ref<Hildur::Shader> m_ColorShader = m_ShaderLibrary.Load("assets/shaders/Color.glsl");


	m_Texture = Hildur::Texture2D::Create("assets/textures/ChernoLogo.png");
	m_Texture2 = Hildur::Texture2D::Create("assets/textures/Ricardo_Milos.png");

	m_Material = Hildur::Material::Create(m_PhongShader);
	//m_Material->SetProperty("ambient", glm::vec3(0.6f));
	//m_Material->SetProperty("diffuse", glm::vec3(0.6f));
	//m_Material->SetProperty("specular", glm::vec3(0.6f));
	m_FlatColorMaterial = Hildur::Material::Create(m_FlatColorShader);

	m_TextureShader->Bind();
	m_TextureShader->SetInt("u_Texture", 1);
	m_TextureShader->SetInt("u_Texture2", 2);


	m_Model = Hildur::Model::Create("assets/models/shaderball.obj");
	m_CubeMesh = Hildur::Model::Create("assets/models/cube.obj");

	m_ObjectRot.x = 180;
	m_ObjectPos = glm::vec3(0.0f, 0.0f, 5.0f);
	LightPos = glm::vec3(-10.f, 0.0f, -10.0f);
	LightColor = glm::vec3(0.1f, 0.6f, 0.9f);

	m_CameraController.SetPosition(glm::vec3(0.0f, 0.0f, -4.0f));
}

Sandbox3D::~Sandbox3D()
{

}

void Sandbox3D::OnAttach()
{

}

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Hildur::Timestep ts)
{
	//Update
	m_CameraController.OnUpdate(ts);

	//Material Uniforms
	m_Material->SetProperty("u_ViewPos", m_CameraController.GetPosition());
	m_Material->SetProperty("u_LightPos", LightPos);
	m_Material->SetProperty("u_LightColor", LightColor);
	m_Material->UpdateUniforms();
	m_FlatColorMaterial->SetProperty("u_Color", glm::vec4(LightColor, 1.0f));
	m_FlatColorMaterial->UpdateUniforms();


	m_ObjectRot.y += translationSpeed * ts * 20;
	glm::mat4 lightTransform = glm::translate(glm::mat4(1.0f), LightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


	//Render
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_ObjectRot.x), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0F), glm::radians(m_ObjectRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

	glm::mat4 transform = rotation * scale * position;

	Hildur::RenderCommand::SetClearColor({ 0.2f, 0.22f, 0.25f, 1 });
	Hildur::RenderCommand::Clear();

	Hildur::Renderer::BeginScene(m_CameraController.GetCamera());

	Hildur::Renderer::Submit(m_Material, m_Model->GetMeshes()[0], transform);
	Hildur::Renderer::Submit(m_FlatColorMaterial, m_CubeMesh->GetMeshes()[0], lightTransform);

	Hildur::Renderer::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
	Hildur::Ref<Hildur::Shader> m_TextureShader;
	Hildur::Ref<Hildur::Shader> m_PhongShader;
	Hildur::Ref<Hildur::Shader> m_LightShader;
	Hildur::Ref<Hildur::Shader> m_FlateColorShader;


	ImGui::Begin("Debug settings");

	if (ImGui::Button("Reload Main Shader")) 
	{
		m_TextureShader = m_ShaderLibrary.Reload("assets/shaders/Texture.glsl");
		m_PhongShader = m_ShaderLibrary.Reload("assets/shaders/Phong.glsl");
		m_LightShader = m_ShaderLibrary.Reload("assets/shaders/Light.glsl");
		m_FlateColorShader = m_ShaderLibrary.Reload("assets/shaders/FlatColor.glsl");
	}

	ImGui::SliderFloat("Ambient", &ambient, 0.0f, 50.0f);
	ImGui::SliderFloat("Diffuse", &diffuse, 0.0f, 50.0f);
	ImGui::SliderFloat("Specular", &specular, 0.0f, 50.0f);

	ImGui::SliderFloat3("Light Position", glm::value_ptr(LightPos), -100.0f, 100.0f);
	ImGui::ColorEdit3("Light Color", glm::value_ptr(LightColor));

	ImGui::SliderFloat("RotationSpeed", &translationSpeed, 0.0f, 50.0f);

	m_Material->SetProperty("ambient", glm::vec3(ambient));
	m_Material->SetProperty("diffuse", glm::vec3(diffuse));
	m_Material->SetProperty("specular", glm::vec3(specular));

	ImGui::End();
}

void Sandbox3D::OnEvent(Hildur::Event& e)
{
	m_CameraController.OnEvent(e);
}
