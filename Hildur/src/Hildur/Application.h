#pragma once

#include "Core.h"

#include "Events/Event.h"
#include "Hildur/Window.h"
#include "Hildur/LayerStack.h"
#include "Hildur/Events/ApplicationEvent.h"

#include "Hildur/Core/Timestep.h"

#include "Hildur/ImGui/imGuiLayer.h"


namespace Hildur {

	class HILDUR_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:

		bool OnWindowClose(WindowCloseEvent& e);

		void DockUpdate();

	private:

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0;

		//Test FBO

		uint32_t fbo;
		uint32_t texture;
		uint32_t depthrenderbuffer;

	private:

		static Application* s_Instance;

	};

	//To be defined in CLIENT
	Application* CreateApplication();

}
