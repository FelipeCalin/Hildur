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
		props.PosX = m_Config.profile.PosX;
		props.PosY = m_Config.profile.PosY;

		if (m_Config.profile.fullscreen)
		{
			props.Width = 1960; // TODO: When "fulscreen" set width and height to display width and height, abstract
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
		//m_ViewportWidth = m_Window->GetWidth();
		//m_ViewportHeight = m_Window->GetHeight();
		m_Viewport.Init(m_Window->GetWidth(), m_Window->GetHeight());

		/// Object ID FrameBuffer /////////////////////////////////////

		//m_IDFrameBuffer = Hildur::FrameBuffer::Create(m_ViewportWidth, m_ViewportHeight);
		//m_IDFrameBuffer->AddTextureAttachment("color");
		//m_IDFrameBuffer->AddDepthBufferAttachment();
		//m_IDFrameBuffer->Ready();

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
				m_Viewport.GetIDFrameBuffer()->Clear();

				m_FrameBuffer->Enable();
				{
					HR_PROFILE_SCOPE("3D Scene Rendering")

					Hildur::Renderer::Prep();
					Hildur::Renderer::RenderQueue();
					Hildur::Renderer::End();
				}

				{
					HR_PROFILE_SCOPE("2D Scene Rendering")

						//Hildur::Renderer2D::Prep();
						//Hildur::Renderer2D::RenderQueue();
						//Hildur::Renderer2D::DrawLine({ -0.25f, -0.25f }, { 0.25f, 0.25f });
				}
				m_FrameBuffer->Disable();

				m_Viewport.GetIDFrameBuffer()->Enable();
				{
					HR_PROFILE_SCOPE("Object ID Rendering")

					Hildur::Renderer::RenderID();
				}
				m_Viewport.GetIDFrameBuffer()->Disable();

				//{
				//	HR_PROFILE_SCOPE("Gizmo Rendering")

				//	Hildur::Renderer::RenderGizmos();
				//}

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
		distpatcher.Dispatch<Hildur::WindowMoveEvent>(BIND_EVENT_FN(OnWindowMove));
		distpatcher.Dispatch<Hildur::WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		distpatcher.Dispatch<Hildur::MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseClick));
		distpatcher.Dispatch<Hildur::MouseScrolledEvent>(BIND_EVENT_FN(OnMouseScrolled));

		m_Viewport.OnEvent(e);

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

	bool Editor::OnWindowMove(Hildur::WindowMoveEvent& e)
	{
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
		/*HR_CORE_TRACE("Mouse Click on: {0}, {1}; Is on Viewport: {2} \n Viewport Coords are: {3}, {4}; Window position: {5}, {6}",
			Hildur::Input::GetMouseX() + GetWindow().GetPositionX(), Hildur::Input::GetMouseY() + GetWindow().GetPositionY(),
			m_Viewport.IsMouseInViewport(), m_Viewport.GetMouseViewportPos().x, m_Viewport.GetMouseViewportPos().y, GetWindow().GetPositionX(), GetWindow().GetPositionY());*/

		return false;
	}

	bool Editor::OnMouseScrolled(Hildur::MouseScrolledEvent& e)
	{
		return false;
	}

	void Editor::RenderEditor()
	{
		ImGui::ShowDemoWindow();

		//Viewport
		m_Viewport.Render(m_FrameBuffer, GetWindow());
		m_SelectedEntity = m_Viewport.GetSelectedEntity();


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
			ImGui::Checkbox("Enable", &m_InspectedEntity->m_IsEnabled);
			m_InspectedEntity->m_Transform->RenderInspector();
			if (m_InspectedEntity->HasComponent<Hildur::Camera>())
				m_InspectedEntity->GetComponent<Hildur::Camera>()->RenderInspector();
			if (m_InspectedEntity->HasComponent<Hildur::DirectionalLight>())
				m_InspectedEntity->GetComponent<Hildur::DirectionalLight>()->RenderInspector();
			if (m_InspectedEntity->HasComponent<Hildur::PointLight>())
				m_InspectedEntity->GetComponent<Hildur::PointLight>()->RenderInspector();
			if (m_InspectedEntity->HasComponent<Hildur::MeshRenderer>())
			{
				m_InspectedEntity->GetComponent<Hildur::MeshRenderer>()->RenderInspector();
				if (ImGui::Button("Load model"))
					m_ExploreFSInspector = true;
			}
		}
		ImGui::End();
		std::string modelPath;
		if (m_InspectorFileBrowser.render(m_ExploreFSInspector, modelPath))
		{
			// The "path" string will hold a valid file path here.
			HR_CORE_WARN("Opened Model in location: {0}", modelPath);

			m_InspectedEntity->GetComponent<Hildur::MeshRenderer>()->SetMesh(Hildur::Model::Create(modelPath)->GetMeshes()[0]);
			m_ExploreFSInspector = false;
		}

		//Asset Manager
		ImGui::Begin("Asset Manager");
		ImGui::End();

		std::string path;
		if (fileBrowser.render(m_IsImportClicked, path)) 
		{
			// The "path" string will hold a valid file path here.
			HR_CORE_ERROR("Opened file in location: {0}", path);
		}


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