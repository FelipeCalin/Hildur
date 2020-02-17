#include "hrpcheaders.h"
#include "Application.h"

#include "Core.h"
#include "Input.h"

#include "Hildur/Renderer/Renderer.h"
#include "Hildur/Renderer/RenderCommand.h"

#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Hildur {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {

		HR_CORE_ASSERT(!s_Instance, "Application already exists")
		s_Instance = this;

		//m_Window = Window::Create();
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_Window->SetVSync(false);

		Renderer::Init();

		//m_ImGuiLayer = std::make_unique<ImGuiLayer>();
		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);


		//test FBO

		
		// The framebuffer
		glGenFramebuffers(1, &fbo);
		Hildur::RenderCommand::SetRenderTarget(fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		
		//The texture
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1960, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		// The depth buffer
		glGenRenderbuffers(1, &depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1960, 1080);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			HR_ASSERT(false, "Could not create FrameBuffer object");
		} else {
			HR_INFO("FrameBuffer object created");
		}
		
		
	}

	Application::~Application() {



	}

	void Application::OnEvent(Event& e) {

		EventDispatcher distpatcher(e);
		distpatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//HR_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {

			(*--it)->OnEvent(e);
			if (e.Handled)
				break;

		}

	}

	void Application::Run() {

		while (m_Running) {

			float time = (float)glfwGetTime(); //TODO Abstract!!
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			m_ImGuiLayer->Begin();
			DockUpdate();
			for (Layer* ImGuiLayer : m_LayerStack)
				ImGuiLayer->OnImGuiRender();

			//Viewport
			ImGui::Begin("ViewPort");

			int width = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
			int height = ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y;

			glViewport(0, 0, ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);

			ImGui::Image((void*)(intptr_t)texture, ImVec2(width, height), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));

			ImGui::End();


			ImGui::Begin("Hierarchy");
			ImGui::End();

			m_ImGuiLayer->End();

			m_Window->OnUpdate();

		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {

		m_Running = false;

		return true;

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

	//Layer stack handling
	void Application::PushLayer(Layer* layer) {

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();

	}

	void Application::PushOverlay(Layer* layer) {

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

}