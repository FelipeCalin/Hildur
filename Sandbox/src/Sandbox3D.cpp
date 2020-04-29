#include "Sandbox3D.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox3D::Sandbox3D()
	:Layer("Sandbox3D"), m_CameraController(45.0f, 16.0f / 9.0f, true), m_ObjectRot(0.0f, 0.0f, 0.0f)
{
	//Shader

	Hildur::Ref<Hildur::Shader> phongShader = m_ShaderLibrary.Load("assets/shaders/Phong.glsl");
	Hildur::Ref<Hildur::Shader> lightShader = m_ShaderLibrary.Load("assets/shaders/Light.glsl");
	Hildur::Ref<Hildur::Shader> colorShader = m_ShaderLibrary.Load("assets/shaders/Color.glsl");
	Hildur::Ref<Hildur::Shader> depthShader = m_ShaderLibrary.Load("assets/shaders/Depth.glsl");


	m_Texture1 = Hildur::Texture2D::Create("assets/textures/Checkerboard.png");
	m_Texture2 = Hildur::Texture2D::Create("assets/textures/Fingerprints1.jpg");
	m_Texture3 = Hildur::Texture2D::Create("assets/textures/ShaderBallEmission.png");

	m_Material = Hildur::Material::Create(phongShader);
	m_Material->SetProperty("albedoMap", 1U);
	m_Material->SetProperty("roughnessMap", 2U);
	//m_Material->SetProperty("ambient", glm::vec3(0.6f));
	//m_Material->SetProperty("diffuse", glm::vec3(0.6f));
	//m_Material->SetProperty("specular", glm::vec3(0.6f));
	m_LightMaterial = Hildur::Material::Create(lightShader);

	
	m_Model = Hildur::Model::Create("assets/models/Shaderball.obj");
	m_CubeModel = Hildur::Model::Create("assets/models/cube.obj");

	m_ObjectRot.x = 180;
	m_ObjectPos = glm::vec3(0.0f, 0.0f, 5.0f);
	lightPos = glm::vec3(-2.f, 0.0f, -2.0f);
	lightDir = glm::vec3(0.1f, 0.6f, 0.9f);
	lightCol = glm::vec3(1.0f, 1.0f, 1.0f);

	m_CameraController.SetPosition(glm::vec3(0.0f, 0.0f, -10.0f));


	//Framebuffer Test

	TestFramebuffer = Hildur::FrameBuffer::Create(1.0f);
	TestFramebuffer->AddTextureAttachment("color");
	TestFramebuffer->AddDepthTextureAttachment("depth");
	TestFramebuffer->Ready();
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
	TestFramebuffer->Enable();
	//Update
	m_CameraController.OnUpdate(ts);

	//Material Uniforms
	m_Material->SetProperty("u_ViewPos", m_CameraController.GetPosition());
	m_Material->UpdateUniforms();
	m_LightMaterial->UpdateUniforms();

	//Texture Binding
	m_Texture1->Bind(1);
	m_Texture2->Bind(2);
	m_Texture3->Bind(3);

	m_ObjectRot.y += translationSpeed * ts * 20;
	glm::mat4 lightTransform = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));

	//Render
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_ObjectRot.x), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0F), glm::radians(m_ObjectRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

	glm::mat4 transform = rotation * scale * position;

	Hildur::RenderCommand::SetClearColor({ 0.2f, 0.22f, 0.25f, 1 });
	Hildur::RenderCommand::Clear();

	Hildur::Renderer::BeginScene(m_CameraController.GetCamera());

	//Hildur::Renderer::Submit(m_Material, m_Model->GetMeshes()[0], transform);
	Hildur::Renderer::Submit(m_LightMaterial, m_CubeModel->GetMeshes()[0], lightTransform);

	glm::vec3 cubePositions[10] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 modelTransform = glm::mat4(1.0f);
		float angle = 20.0f * i;
		modelTransform = glm::translate(glm::mat4(1.0f), cubePositions[i])
			* glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(0.1));
		Hildur::Renderer::Submit(m_Material, m_Model->GetMeshes()[0], modelTransform);
	}

	Hildur::Renderer::EndScene();

	TestFramebuffer->Disable();

	//TestFramebuffer->BlitToScreen("");
	//TestFramebuffer->Render("color");
}

void Sandbox3D::OnImGuiRender()
{
	Hildur::Ref<Hildur::Shader> phongShader;
	Hildur::Ref<Hildur::Shader> lightShader;
	Hildur::Ref<Hildur::Shader> colorShader;
	Hildur::Ref<Hildur::Shader> depthShader;

	ImGui::Begin("ViewPort");
	ImGui::GetWindowDrawList()->AddImage((void*)TestFramebuffer->GetAttachment("color")->rendererID, ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowContentRegionMax().x, ImGui::GetCursorScreenPos().y + ImGui::GetWindowContentRegionMax().y), ImVec2(0, 1), ImVec2(1, 0));
	//ImGui::Image((void*)TestFramebuffer->GetAttachment("color")->rendererID, ImVec2(TestFramebuffer->GetWidth(), TestFramebuffer->GetHeight()));
	
	ImGui::End();

	ImGui::Begin("SIZE");

	ImGui::DragFloat2("Size", (float*)&ImGui::GetCursorScreenPos());

	ImGui::End();

	ImGui::Begin("Debug settings");

	if (ImGui::Button("Reload Main Shader")) 
	{
		phongShader = m_ShaderLibrary.Reload("assets/shaders/Phong.glsl");
		lightShader = m_ShaderLibrary.Reload("assets/shaders/Light.glsl");
		colorShader = m_ShaderLibrary.Reload("assets/shaders/Color.glsl");
		depthShader = m_ShaderLibrary.Reload("assets/shaders/Depth.glsl");

		m_Material = Hildur::Material::Create(phongShader);
	}

	ImGui::SliderFloat("Ambient", &objectAmbient, 0.0f, 15.0f);
	ImGui::SliderFloat("Diffuse", &objectDiffuse, 0.0f, 15.0f);
	ImGui::SliderFloat("Specular", &objectSpecular, 0.0f, 15.0f);
	ImGui::SliderFloat("Shininess", &objectShininess, 0.0f, 15.0f);
	ImGui::Spacing();
	ImGui::SliderFloat3("Light Position", glm::value_ptr(lightPos), -10.0f, 10.0f);
	ImGui::SliderFloat3("Light Direction", glm::value_ptr(lightDir), -10.0f, 10.0f);
	ImGui::ColorEdit3("Light Color", glm::value_ptr(lightCol));

	ImGui::SliderFloat("Light Ambient", &lightAmbient, 0.0f, 15.0f);
	ImGui::SliderFloat("Light Diffuse", &lightDiffuse, 0.0f, 15.0f);
	ImGui::SliderFloat("Light Specular", &lightSpecular, 0.0f, 15.0f);

	ImGui::SliderFloat("Light constant", &lightConstant, 0.0f, 2.0f);
	ImGui::SliderFloat("Light linear", &lightLinear, 0.0f, 2.0f);
	ImGui::SliderFloat("Light quadratic", &lightQuadratic, 0.0f, 2.0f);

	ImGui::SliderFloat("Light CutOff", &lightCutOff, 0.0f, 100.0f);
	ImGui::SliderFloat("Light OuterCutOff", &lightOuterCutOff, 0.0f, 100.0f);

	ImGui::Spacing();

	ImGui::SliderFloat("RotationSpeed", &translationSpeed, 0.0f, 50.0f);

	m_Material->SetProperty("material.ambient", glm::vec3(objectAmbient));
	m_Material->SetProperty("material.diffuse", glm::vec3(objectDiffuse));
	m_Material->SetProperty("material.specular", glm::vec3(objectSpecular));
	m_Material->SetProperty("material.shininess", objectShininess);


	m_Material->SetProperty("pointLight.position", lightPos);
	m_Material->SetProperty("pointLight.direction", lightDir);
	m_Material->SetProperty("pointLight.ambient", glm::vec3(lightAmbient));
	m_Material->SetProperty("pointLight.diffuse", glm::vec3(lightDiffuse) * lightCol);
	m_Material->SetProperty("pointLight.specular", glm::vec3(lightSpecular) * lightCol);
	m_Material->SetProperty("pointLight.constant", lightConstant);
	m_Material->SetProperty("pointLight.linear", lightConstant);
	m_Material->SetProperty("pointLight.quadratic", lightQuadratic);
	m_Material->SetProperty("pointLight.cutOff", glm::cos(glm::radians(lightCutOff)));
	m_Material->SetProperty("pointLight.outerCutOff", glm::cos(glm::radians(lightOuterCutOff)));

	m_LightMaterial->SetProperty("lightColor", lightCol);

	ImGui::End();
}

void Sandbox3D::OnEvent(Hildur::Event& e)
{
	m_CameraController.OnEvent(e);

	Hildur::EventDispatcher dispatcher(e);

	dispatcher.Dispatch<Hildur::WindowResizeEvent>(HR_BIND_EVENT_FN(Sandbox3D::OnWindowResize));
	dispatcher.Dispatch<Hildur::WindowCloseEvent>(HR_BIND_EVENT_FN(Sandbox3D::OnWindowClose));

}

bool Sandbox3D::OnWindowResize(Hildur::WindowResizeEvent& e)
{
	TestFramebuffer->ResizeAll();

	return false;
}

bool Sandbox3D::OnWindowClose(Hildur::WindowCloseEvent& e)
{


	return false;
}