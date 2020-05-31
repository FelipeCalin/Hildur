#include "hrpcheaders.h"
#include "Application.h"

#include "Hildur/Core/Core.h"
#include "Input.h"

#include "Hildur/Core/System/Renderer.h"
#include "Hildur/Core/System/Renderer2D.h"
#include "Hildur/Renderer/RenderCommand.h"
#include "Hildur/Core/Entity.h"

#include "Hildur/Component/Transform.h"
#include "Hildur/Component/Camera.h"
#include "Hildur/Component/Renderable.h"
#include "Hildur/Component/MeshRenderer.h"
#include "Hildur/Component/LightEmitter.h"
#include "Hildur/Component/DirectionalLight.h"
#include "Hildur/Component/PointLight.h"

#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>


namespace Hildur {


#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)


	Application* Application::s_Instance = nullptr;


	Application::Application()
	{
		HR_PROFILE_FUNCTION()


	}

	Application::~Application()
	{
		HR_PROFILE_FUNCTION()

	}

	void Application::Init()
	{
		HR_PROFILE_FUNCTION();
		/// Configuration /////////////////////////////////////////////

		m_Config.readConfig();

		WindowProps props;
		props.Title = m_Config.profile.appName;
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

		m_Window = Window::Create(props);
		m_SceneManager = SceneManager::Create();


		/// Window initialization /////////////////////////////////////

		HR_CORE_ASSERT(!s_Instance, "Application already exists")
			s_Instance = this;

		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_Window->SetVSync(m_Config.profile.vsync);


		/// Renderer initialization ///////////////////////////////////
		Renderer::Init();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

		/// Debug ImGui Layer initialization //////////////////////////
		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
	}

	void Application::SetScenes(std::map<std::string, Scene*>& sceneMap)
	{
		HR_PROFILE_FUNCTION()

		m_SceneManager->SetSceneMap(sceneMap);
	}

	void Application::Run() 
	{
		/// Main Loop /////////////////////////////////////////////////

		HR_PROFILE_FUNCTION()

		while (m_Running)
		{
			HR_PROFILE_SCOPE("RunLoop")

			float time = (float)glfwGetTime(); //TODO Abstract!!
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					HR_PROFILE_SCOPE("Render Preparation")

					Hildur::RenderCommand::Clear();
				}

				{
					HR_PROFILE_SCOPE("3D Scene Rendering")

					Renderer::Prep();
					Renderer::RenderQueue();
					Renderer::End();
				}

				{
					HR_PROFILE_SCOPE("2D Scene Rendering")

					Renderer2D::Prep();
					Renderer2D::RenderQueue();
				}

				{
					HR_PROFILE_SCOPE("LayerStack OnUpdates")

						for (Layer* layer : m_LayerStack)
							layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					HR_PROFILE_SCOPE("LayerStack OnImGuiRender")

						for (Layer* ImGuiLayer : m_LayerStack)
							ImGuiLayer->OnImGuiRender();
				}


				//Draw scene hierarchy
				ImGui::Begin("Scene");
				ImGui::SameLine();
				ImGui::TextUnformatted(m_SceneManager->GetCurrentName().c_str());
				ImGui::End();

				ImGui::ShowDemoWindow();

				m_SceneManager->DrawSceneList();

				m_ImGuiLayer->End();
			}

			if (m_SceneManager->SceneQueued())
			{
				m_SceneManager->UnloadScene(SceneManager::GetCurrentScene());

				m_SceneManager->LoadQueuedScene();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		HR_PROFILE_FUNCTION()

		EventDispatcher distpatcher(e);
		distpatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		distpatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//HR_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) 
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) 
	{
		HR_PROFILE_FUNCTION()

		if (e.GetWidth() == 0 || e.GetHeight() == 0) 
		{
			m_Minimized = true;

			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		HR_PROFILE_FUNCTION()

		m_Running = false;

		return true;
	}

	 /// LayerStack Handling ///////////////////////////////////////

	void Application::PushLayer(Layer* layer)
	{
		HR_PROFILE_FUNCTION()

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		HR_PROFILE_FUNCTION()

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}


}