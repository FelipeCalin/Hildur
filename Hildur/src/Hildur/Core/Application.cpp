#include "hrpcheaders.h"
#include "Application.h"

#include "Hildur/Core/Core.h"
#include "Input.h"

#include "Hildur/Core/System/Renderer.h"
#include "Hildur/Renderer/RenderCommand.h"
#include "Hildur/Core/Entity.h"

#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Hildur {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {

		/// Configuration /////////////////////////////////////////////

		m_Config.readConfig();

		WindowProps props;
		props.Title = m_Config.profile.appName;

		if (m_Config.profile.fullscreen) {

			props.Width = 1960;
			props.Height = 1080;

		}
		else {

			props.Width = m_Config.profile.width;
			props.Height = m_Config.profile.height;

		}

		/// System Singleton Instacing ////////////////////////////////

		m_Window = std::unique_ptr<Window>(Window::Create(props));
		m_SceneManager = std::unique_ptr<SceneManager>();


		 /// Window initialization /////////////////////////////////////

		HR_CORE_ASSERT(!s_Instance, "Application already exists")
		s_Instance = this;

		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_Window->SetVSync(m_Config.profile.vsync);


		/// Renderer initialization ///////////////////////////////////
		Renderer::Init();


		/// Debug ImGui Layer initialization //////////////////////////
		//m_ImGuiLayer = std::make_unique<ImGuiLayer>();
		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);


		
		/// Viewport FBO initialization ///////////////////////////////
		
		// The framebuffer
		m_FBO = FrameBuffer::Create(1960, 1080);

		// The depth buffer
		/*glGenRenderbuffers(1, &depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1960, 1080);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);*/

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBO->GetTexture(), 0);

		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		// UnBind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		
	}

	Application::~Application() {



	}

	void Application::Init(std::map<std::string, Scene*>& sceneMap) {

		m_SceneManager->SetSceneMap(sceneMap);

	}

	void Application::OnEvent(Event& e) {

		EventDispatcher distpatcher(e);
		distpatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		distpatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		//HR_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {

			(*--it)->OnEvent(e);
			if (e.Handled)
				break;

		}

	}

	void Application::Run() {

		/// Main Loop /////////////////////////////////////////////////
	   ///////////////////////////////////////////////////////////////

		while (m_Running) {

			float time = (float)glfwGetTime(); //TODO Abstract!!
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;


			m_FBO->Bind();

			if (!m_Minimized) {

				//Update Layers
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);

			}

			m_FBO->UnBind();

			m_ImGuiLayer->Begin();
			DockUpdate();
			for (Layer* ImGuiLayer : m_LayerStack)
				ImGuiLayer->OnImGuiRender();

			//Viewport
			ImGui::Begin("ViewPort");

			m_ViewportWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
			m_ViewportHeight = ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y;

			glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);

			ImGui::GetWindowDrawList()->AddImage(
				(void*)(intptr_t)m_FBO->GetTexture(),
				ImVec2(ImGui::GetCursorScreenPos()),
				ImVec2(ImGui::GetCursorScreenPos().x + m_ViewportWidth,
					ImGui::GetCursorScreenPos().y + m_ViewportHeight), ImVec2((1.0f / 1960.0f) * m_ViewportWidth, 0), ImVec2(0, (1.0f / 1080.0f) * m_ViewportHeight));

			ImGui::End();


			ImGui::Begin("Current Scene");
			//std::string currentSceneName = m_SceneManager->GetCurrentName();
			ImGui::TextUnformatted(m_SceneManager->GetCurrentName().c_str());
			ImGui::End();

			m_SceneManager->DrawSceneList();

			ImGui::Begin("Hierarchy");
			ImGui::End();

			m_ImGuiLayer->End();

			m_SceneManager->LoadQueuedScene();

			m_Window->OnUpdate();

		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {

		m_Running = false;

		return true;

	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {

		if (e.GetWidth() == 0 || e.GetHeight() == 0) {

			m_Minimized = true;

			return false;

		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;

	}

	void Application::DockUpdate() {

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

	 /// LayerStack Handling ///////////////////////////////////////
	///////////////////////////////////////////////////////////////

	void Application::PushLayer(Layer* layer) {

		m_LayerStack.PushLayer(layer);

	}

	void Application::PushOverlay(Layer* layer) {

		m_LayerStack.PushOverlay(layer);

	}

}