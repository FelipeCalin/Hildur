#pragma once

#include "Hildur/Core/Core.h"

#include "Hildur/Core/Config.h"

#include "Hildur/Events/Event.h"
#include "Hildur/Core/Window.h"
#include "Hildur/Core/LayerStack.h"
#include "Hildur/Events/ApplicationEvent.h"
#include "Hildur/Renderer/Buffer.h"
#include "Hildur/Resource/FrameBuffer.h"

#include "Hildur/Core/System/SceneManager.h"

#include "Hildur/Core/Timestep.h"

#include "Hildur/ImGui/imGuiLayer.h"


namespace Hildur {


	class HILDUR_API Application
	{
	public:

		Application();
		virtual ~Application();

		virtual void Init();
		virtual void SetScenes(std::map<std::string, Scene*>& sceneMap);
		virtual void Run();

		virtual void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

		virtual bool IsInitOverriden() const { return false; }

		//inline static Timestep& GetTimestep() {return }

	private:

		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

	protected:

		Config m_Config;

		Scope<SceneManager> m_SceneManager;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0;

		//Test FBO

		//uint32_t m_ViewportWidth;
		//uint32_t m_ViewportHeight;
		//Ref<FrameBuffer> m_FBO;

		bool m_OverrideInit = false;
		
	protected:

		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();


}
