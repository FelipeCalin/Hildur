#include "Sandbox3D.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox3D::Sandbox3D()
	:Layer("Sandbox3D"), m_CameraController(45.0f, 16.0f / 9.0f, true), m_ObjectRot(0.0f, 0.0f, 0.0f)
{
	//Skybox

	std::vector<std::string> sea = {
	"assets/skybox/right.jpg",
	"assets/skybox/left.jpg",
	"assets/skybox/top.jpg",
	"assets/skybox/bottom.jpg",
	"assets/skybox/front.jpg",
	"assets/skybox/back.jpg",
	};

	m_CubeMap = Hildur::CubeMap::Create(sea);

	Hildur::Renderer::SetSkybox(m_CubeMap);

	//Shader

	Hildur::Ref<Hildur::Shader> phongShader = m_ShaderLibrary.Load("assets/shaders/Phong.glsl");
	Hildur::Ref<Hildur::Shader> lightShader = m_ShaderLibrary.Load("assets/shaders/Light.glsl");
	Hildur::Ref<Hildur::Shader> colorShader = m_ShaderLibrary.Load("assets/shaders/Color.glsl");
	Hildur::Ref<Hildur::Shader> postPShader = m_ShaderLibrary.Load("assets/shaders/FrameBufferQuad.glsl");


	m_Texture1 = Hildur::Texture2D::Create("assets/textures/Checkerboard.png");
	m_Texture2 = Hildur::Texture2D::Create("assets/textures/Fingerprints1.jpg");
	m_Texture3 = Hildur::Texture2D::Create("assets/textures/ShaderBallEmission.png");

	m_Material = Hildur::Material::Create(phongShader);
	m_Material->SetProperty("albedoMap", 1U);
	m_Material->SetProperty("roughnessMap", 2U);
	m_Material->SetProperty("emissionMap", 3U);
	m_LightMaterial = Hildur::Material::Create(colorShader);
	
	m_Model = Hildur::Model::Create("assets/models/ShaderBall.fbx");
	m_CubeModel = Hildur::Model::Create("assets/models/cube.obj");

	m_ObjectRot.x = 180;
	m_ObjectPos = glm::vec3(0.0f, 0.0f, 5.0f);
	lightPos = glm::vec3(-2.f, 0.0f, -2.0f);
	lightDir = glm::vec3(0.1f, 0.6f, 0.9f);
	lightCol = glm::vec3(0.5019f, 0.2902f, 0.7294f);

	m_CameraController.SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));


	//Framebuffer Test

	std::vector<glm::vec3> quadVertices
	{
		{-1.0f, 1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}
	};

	TestMesh = Hildur::Mesh::Create(quadVertices);

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
	if (Hildur::Input::IsMouseButtonPressed(0))
	{
		m_CameraXoffset = (Hildur::Input::GetMouseY() - m_MouseXLast);
		m_CameraYoffset = (Hildur::Input::GetMouseX() - m_MouseYLast);
		m_CameraXRot += m_CameraXoffset;
		m_CameraYRot += m_CameraYoffset;
		m_CameraController.SetRotation(glm::vec3(glm::radians(m_CameraXRot), glm::radians(m_CameraYRot), 0.0f));
	}
	else
	{
		m_CameraXoffset = 0.0f;
		m_CameraYoffset = 0.0f;
	}

	m_MouseXLast = Hildur::Input::GetMouseY();
	m_MouseYLast = Hildur::Input::GetMouseX();
	HR_INFO("{0}, {1}, {2}", m_CameraController.GetCamera().GetForwardDir().x, m_CameraController.GetCamera().GetForwardDir().y, m_CameraController.GetCamera().GetForwardDir().z);

	// RENDERING

	//Enable main framebuffer
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
	auto postPShader = m_ShaderLibrary.Get("FrameBufferQuad");
	postPShader->SetInt("u_ScreenTexture", 0);
	TestFramebuffer->GetAttachment("color")->Bind(0);
	//Hildur::Renderer::Submit(postPShader, TestMesh, glm::mat4(0.0f));
}

void Sandbox3D::OnImGuiRender()
{
	DockUpdate();

	Hildur::Ref<Hildur::Shader> phongShader;
	Hildur::Ref<Hildur::Shader> lightShader;
	Hildur::Ref<Hildur::Shader> colorShader;
	Hildur::Ref<Hildur::Shader> depthShader;

	ImGui::Begin("ViewPort");
	Hildur::RenderCommand::SetViewport(0, 0, ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);
	Hildur::WindowResizeEvent cameraEvent(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);
	m_CameraController.OnEvent(cameraEvent);
	ImGui::GetWindowDrawList()->AddImage((void*)TestFramebuffer->GetAttachment("color")->rendererID, ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowContentRegionMax().x, ImGui::GetCursorScreenPos().y + ImGui::GetWindowContentRegionMax().y), ImVec2(0, 1), ImVec2(1, 0));
	//ImGui::Image((void*)TestFramebuffer->GetAttachment("color")->rendererID, ImVec2(TestFramebuffer->GetWidth(), TestFramebuffer->GetHeight()));
	//TestFramebuffer->Resize(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);
	ImGui::End();

	ImGui::Begin("Debug settings");

	if (ImGui::Button("Reload Shaders")) 
	{
		phongShader = m_ShaderLibrary.Reload("assets/shaders/Phong.glsl");
		lightShader = m_ShaderLibrary.Reload("assets/shaders/Light.glsl");
		colorShader = m_ShaderLibrary.Reload("assets/shaders/Color.glsl");
		depthShader = m_ShaderLibrary.Reload("assets/shaders/Depth.glsl");

		m_Material = Hildur::Material::Create(phongShader);
		m_LightMaterial = Hildur::Material::Create(colorShader);
	}

	if (ImGui::Button("Reload Models"))
	{
		m_Model = Hildur::Model::Create("assets/models/ShaderBall.fbx");
		m_CubeModel = Hildur::Model::Create("assets/models/cube.obj");
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

	m_LightMaterial->SetProperty("u_Color", glm::vec4(lightCol, 1.0f));

	ImGui::End();
}

void Sandbox3D::OnEvent(Hildur::Event& e)
{
	if (e.GetEventType() != Hildur::EventType::WindowResize)
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

void Sandbox3D::DockUpdate() {

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	static bool open;
	bool* p_open = &open;


	//Docking
	ImGui::Begin("DockSpace", p_open, window_flags);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		//if (ImGui::BeginMenu("Hildur Workspace"))
		//{
		//	// Disabling fullscreen would allow the window to be moved to the front of other windows,
		//	// which we can't undo at the moment without finer window depth/z control.
		//	//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

		//	if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
		//	if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
		//	if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
		//	if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
		//	if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
		//	ImGui::Separator();
		//	if (ImGui::MenuItem("Close DockSpace", NULL, false, p_open != NULL))
		//		*p_open = false;
		//	ImGui::EndMenu();
		//}

		ImGui::EndMenuBar();

	}

	ImGui::End();

}