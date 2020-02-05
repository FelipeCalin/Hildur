#include "hrpcheaders.h"
#include "Application.h"

#include "Core.h"
#include "Input.h"

#include "Hildur/Renderer/Renderer.h"
#include "Hildur/Renderer/RenderCommand.h"

#include <imgui.h>

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

		//m_ImGuiLayer = std::make_unique<ImGuiLayer>();
		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);

	}

	Application::~Application() {



	}

	void Application::OnEvent(Event& e) {

		EventDispatcher distpatcher(e);
		distpatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindoClose));

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

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* ImGuiLayer : m_LayerStack)
				ImGuiLayer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

		}

	}

	bool Application::OnWindoClose(WindowCloseEvent& e) {

		m_Running = false;

		return true;

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