#include "Editor.h"
#include <Hildur/Core/EntryPoint.h>

#include "BlankScene.h"

#include <ImGui/imgui.h>
#include <GLFW/include/GLFW/glfw3.h>
#include <GLM/glm/glm.hpp>


#define BIND_EVENT_FN(x) std::bind(&Editor::x, this, std::placeholders::_1)

namespace Editor {


	Editor::Editor()
	{
		Init();
	}

	Editor::~Editor()
	{
	}

	void Editor::Init()
	{
		/// Configuration /////////////////////////////////////////////

		m_Config.readConfig();

		Hildur::WindowProps props;
		props.Title = "Hildur Editor";
		props.IsFullscreen = m_Config.profile.fullscreen;

		if (m_Config.profile.fullscreen)
		{
			props.Width = 1960;
			props.Height = 1080;
		}
		else
		{
			props.Width = m_Config.profile.width;
			props.Height = m_Config.profile.height;
		}

		/// System Singleton Instacing ////////////////////////////////

		m_Window = Hildur::Window::Create(props);
		m_SceneManager = Hildur::SceneManager::Create();


		/// Window initialization /////////////////////////////////////

		HR_CORE_ASSERT(!s_Instance, "Application already exists")
			s_Instance = this;

		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_Window->SetVSync(m_Config.profile.vsync);


		/// Renderer initialization ///////////////////////////////////
		Hildur::Renderer::Init();
		Hildur::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));

		/// Debug ImGui Layer initialization //////////////////////////
		m_ImGuiLayer = new Hildur::ImGuiLayer;
		PushOverlay(m_ImGuiLayer);

		/// Console Initialization ////////////////////////////////////

		m_Console = std::dynamic_pointer_cast<Hildur::ImGuiConsole>(Hildur::Log::GetSinks()[1]);

		/// Viewport FrameBuffer //////////////////////////////////////

		m_FrameBuffer = Hildur::FrameBuffer::Create(m_Window->GetWidth(), m_Window->GetHeight());
		m_FrameBuffer->AddTextureAttachment("color");
		m_FrameBuffer->AddDepthBufferAttachment();
		m_FrameBuffer->Ready();
		m_ViewportWidth = m_Window->GetWidth();
		m_ViewportHeight = m_Window->GetHeight();

		/// Object ID FrameBuffer /////////////////////////////////////

		m_IDFrameBuffer = Hildur::FrameBuffer::Create(m_ViewportWidth, m_ViewportHeight);
		m_IDFrameBuffer->AddTextureAttachment("color");
		m_IDFrameBuffer->AddDepthBufferAttachment();
		m_IDFrameBuffer->Ready();

		//TODO: Fix!!!

		//std::map<std::string, Hildur::Scene*> sceneMap;
		//sceneMap["blank Scene"] = new BlankScene();
		Hildur::Scene* blankScene = new BlankScene();
		m_SceneManager->AddScene("blank Scene", blankScene);
		//m_SceneManager->SetSceneMap(sceneMap);
		m_Window->SetName(std::string("Hildur Editor - \"blank\" Scene"));
		m_SceneManager->LoadScene("blank Scene");
	}

	void Editor::SetScenes(std::map<std::string, Hildur::Scene*>& sceneMap)
	{
		m_SceneManager->SetSceneMap(sceneMap);
	}

	void Editor::Run()
	{
		/// Main Loop /////////////////////////////////////////////////

		HR_PROFILE_FUNCTION()

			while (m_Running)
			{
				HR_PROFILE_SCOPE("Editor RunLoop")

					float time = (float)glfwGetTime(); //TODO Abstract!!
				Hildur::Timestep timestep = time - m_LastFrameTime;
				m_LastFrameTime = time;

				Hildur::RenderCommand::Clear();
				m_FrameBuffer->Clear();
				m_IDFrameBuffer->Clear();

				m_FrameBuffer->Enable();
				{
					HR_PROFILE_SCOPE("3D Scene Rendering")

						Hildur::Renderer::Prep();
					Hildur::Renderer::RenderQueue();
					Hildur::Renderer::End();
				}

				{
					HR_PROFILE_SCOPE("2D Scene Rendering")

						Hildur::Renderer2D::Prep();
					Hildur::Renderer2D::RenderQueue();
				}
				m_FrameBuffer->Disable();

				m_IDFrameBuffer->Enable();
				{
					HR_PROFILE_SCOPE("LayerStack OnUpdates")

						Hildur::Renderer::RenderID();
				}
				m_IDFrameBuffer->Disable();

				{
					HR_PROFILE_SCOPE("LayerStack OnUpdates")

						for (Hildur::Layer* layer : m_LayerStack)
							layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					HR_PROFILE_SCOPE("LayerStack OnImGuiRender")

						for (Hildur::Layer* ImGuiLayer : m_LayerStack)
							ImGuiLayer->OnImGuiRender();
				}

				DockUpdate();
				RenderEditor();
				m_ImGuiLayer->End();

				m_SceneManager->LoadQueuedScene();

				m_Window->OnUpdate();
			}
	}

	void Editor::OnEvent(Hildur::Event& e)
	{
		HR_PROFILE_FUNCTION()

			Hildur::EventDispatcher distpatcher(e);
		distpatcher.Dispatch<Hildur::WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		distpatcher.Dispatch<Hildur::WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		distpatcher.Dispatch<Hildur::MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseClick));
		distpatcher.Dispatch<Hildur::MouseScrolledEvent>(BIND_EVENT_FN(OnMouseScrolled));

		//HR_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Editor::OnWindowResize(Hildur::WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;

			return false;
		}

		m_Minimized = false;

		return false;
	}

	bool Editor::OnWindowClose(Hildur::WindowCloseEvent& e)
	{
		m_WantsToClose = true;

		if (m_CanClose)
		{
			m_Running = false;

			return false;
		}

		return true;
	}

	bool Editor::OnMouseClick(Hildur::MouseButtonPressedEvent& e)
	{
		return false;
	}

	bool Editor::OnMouseScrolled(Hildur::MouseScrolledEvent& e)
	{
		if (m_IsInViewport)
		{
			m_ViewportScroll += e.GetYOffset();

			//Hildur::Transform* camTransform = Hildur::Camera::GetMainCamera()->GetTransform();
			//float camX = sin(camTransform->GetRotation().y) * m_ViewportScroll;
			//float camZ = cos(camTransform->GetRotation().y) * m_ViewportScroll;
			//camTransform->SetPosition({ camX, 0.0f, camZ });
		}

		return false;
	}

	void Editor::RenderEditor()
	{
		ImGui::ShowDemoWindow();


		//Viewport
		ImGui::Begin("Viewport");
		m_ViewportWidth = ImGui::GetWindowContentRegionMax().x;
		m_ViewportHeight = ImGui::GetWindowContentRegionMax().y;
		m_IsInViewport = ImGui::IsWindowHovered();
		ImGui::GetWindowDrawList()->AddImage(
			(void*)(intptr_t)m_FrameBuffer->GetAttachment("color")->rendererID,
			ImVec2(ImGui::GetCursorScreenPos()),
			ImVec2(ImGui::GetCursorScreenPos().x + m_ViewportWidth, ImGui::GetCursorScreenPos().y + m_ViewportHeight));
		if (Hildur::Input::IsMouseButtonPressed(1) && m_IsInViewport && !m_IsMovingInVP)
		{
			m_IsMovingInVP = true;
			m_OriginalMousePos = { Hildur::Input::GetMouseX(), Hildur::Input::GetMouseY() };
			m_ViewportCamPos = Hildur::Renderer::GetCameraPos();
			m_ViewportCamRot = Hildur::Camera::GetMainCamera()->GetTransform()->GetRotation().y;
		}
		else if (Hildur::Input::IsMouseButtonPressed(1) && m_IsMovingInVP)
		{
			float RotX = (Hildur::Input::GetMouseY() - m_OriginalMousePos.y) * 0.01f;
			m_ViewportCamRot += (Hildur::Input::GetMouseX() - m_OriginalMousePos.x) * 0.01f;
			HR_CORE_INFO("RotY: {0}", m_ViewportCamRot);
			Hildur::Transform* camTransform = Hildur::Camera::GetMainCamera()->GetTransform();
			camTransform->SetRotation({ 0.0f, m_ViewportCamRot, 0.0f });
			float camX = sin(-glm::radians(m_ViewportCamRot)) * m_ViewportScroll;
			float camZ = cos(-glm::radians(m_ViewportCamRot)) * m_ViewportScroll;
			float CamY = camTransform->GetPosition().y;
			m_ViewportCamPos += glm::vec3(camX, CamY, camZ);
			camTransform->SetPosition(m_ViewportCamPos);
		}
		else if (!Hildur::Input::IsMouseButtonPressed(1))
		{
			m_IsMovingInVP = false;
		}

		if (m_IsInViewport && Hildur::Input::IsMouseButtonPressed(0))
		{
			m_IDFrameBuffer->Enable();

			uint32_t mousePosViewPortX = ImGui::GetMousePos().x - ImGui::GetCursorScreenPos().x + 8;
			uint32_t mousePosViewPortY = ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y + 8;

			HR_CORE_INFO("{0}, {1}", mousePosViewPortX, mousePosViewPortY);
		}

		Hildur::Renderer::OnWindowResize(m_ViewportWidth, m_ViewportHeight);
		ImGui::End();


		//TEST
		//ImGui::Begin("ID buffer");
		//uint32_t viewportWidth = ImGui::GetWindowContentRegionMax().x;
		//uint32_t viewportHeight = ImGui::GetWindowContentRegionMax().y;
		//ImGui::GetWindowDrawList()->AddImage(
		//	(void*)(intptr_t)m_IDFrameBuffer->GetAttachment("color")->rendererID,
		//	ImVec2(ImGui::GetCursorScreenPos()),
		//	ImVec2(ImGui::GetCursorScreenPos().x + viewportWidth, ImGui::GetCursorScreenPos().y + viewportHeight));
		//ImGui::End();


		//Hierarchy
		ImGui::Begin("Hierarchy");
		if (ImGui::TreeNodeEx(m_SceneManager->GetCurrentName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

			if (m_SceneManager->GetCurrentScene() != nullptr)
			{
				for (Hildur::Entity* entity : m_SceneManager->GetCurrentScene()->GetEntities())
				{
					if (entity->m_Transform->GetParent() == nullptr)
						RenderChild(entity);
				}
			}
			ImGui::TreePop();
		}
		ImGui::End();

		//Inspector
		ImGui::Begin("Inspector");
		if (m_InspectedEntity != nullptr)
		{
			m_InspectedEntity->m_Transform->RenderInspector();
			if (m_InspectedEntity->HasComponent<Hildur::Camera>())
				m_InspectedEntity->GetComponent<Hildur::Camera>()->RenderInspector();
			if (m_InspectedEntity->HasComponent<Hildur::DirectionalLight>())
				m_InspectedEntity->GetComponent<Hildur::DirectionalLight>()->RenderInspector();
			if (m_InspectedEntity->HasComponent<Hildur::PointLight>())
				m_InspectedEntity->GetComponent<Hildur::PointLight>()->RenderInspector();
			if (m_InspectedEntity->HasComponent<Hildur::MeshRenderer>())
				m_InspectedEntity->GetComponent<Hildur::MeshRenderer>()->RenderInspector();
		}
		ImGui::End();

		//Asset Manager
		ImGui::Begin("Asset Manager");

		ImGui::End();

		//Console
		m_Console->OnImGuiRender(&m_UIShowConsole);

		//Are you sure you want to quit?
		if (m_WantsToClose)
			ImGui::OpenPopup("Close Window");

		bool open = true;
		if (ImGui::BeginPopupModal("Close Window", &open))
		{
			ImGui::Text("Are you sure you want to quit?");
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
				m_CanClose = false;
				m_WantsToClose = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
				m_CanClose = true;
				Hildur::WindowCloseEvent& e = Hildur::WindowCloseEvent();
				OnWindowClose(e);
			}
			ImGui::EndPopup();
		}
	}

	void Editor::DockUpdate()
	{
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
		ImGui::Begin("DockSpace Demo", p_open, window_flags);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window"))
			{
				//if(ImGui::MenuItem("Viewport"))
				//	ImGui::Checkbox("Viewport", &m_UIShowViewPort);
				////ImGui::Checkbox("Hierarchy", &m_UIShowHierarchy);
				////ImGui::Checkbox("Inspector", &m_UIShowInspector);
				////ImGui::Checkbox("Asset explorer", &m_UIShowAssetExplorer);
				////ImGui::Checkbox("File manager", &m_UIShowFileManager);
			}
			if (ImGui::BeginMenu("Help"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();

		}

		ImGui::End();
	}

	void Editor::RenderChild(Hildur::Entity* entity)
	{
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

		ImGuiTreeNodeFlags node_flags = base_flags;
		if (m_InspectedEntity == entity)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		if (ImGui::TreeNodeEx(entity->m_Name.c_str(), node_flags))
		{
			if (entity->m_Transform->HasChildren())
			{
				for (Hildur::Transform* child : entity->m_Transform->GetChildren())
				{
					RenderChild(child->GetEntity());
				}
			}
			ImGui::TreePop();
		}

		if (ImGui::IsItemClicked())
			m_InspectedEntity = entity;
	}


}


//Editor Creation

Hildur::Application* Hildur::CreateApplication()
{
	return new Editor::Editor;
}