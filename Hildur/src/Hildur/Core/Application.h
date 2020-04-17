#pragma once

#include "Hildur/Core/Core.h"

#include "Hildur/Core/Config.h"

#include "Hildur/Events/Event.h"
#include "Hildur/Core/Window.h"
#include "Hildur/Core/LayerStack.h"
#include "Hildur/Events/ApplicationEvent.h"
#include "Hildur/Renderer/Buffer.h"

#include "Hildur/Core/System/SceneManager.h"

#include "Hildur/Core/Timestep.h"

#include "Hildur/ImGui/imGuiLayer.h"


namespace Hildur {


	class HILDUR_API Application {

	public:

		Application();
		virtual ~Application();

		void Init(std::map<std::string, Scene*>& sceneMap);
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

		//inline static Timestep& GetTimestep() {return }

	private:

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void DockUpdate();

	private:

		Config m_Config;

		std::unique_ptr<SceneManager> m_SceneManager;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0;

		//Test FBO

		//uint32_t m_ViewportWidth;
		//uint32_t m_ViewportHeight;
		//Ref<FrameBuffer> m_FBO;

	private:

		static Application* s_Instance;

	};

	//To be defined in CLIENT
	Application* CreateApplication();

}
