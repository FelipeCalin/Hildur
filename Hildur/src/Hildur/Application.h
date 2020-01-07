#pragma once

#include "Core.h"

#include "Events/Event.h"
#include "Hildur/Window.h"
#include "Hildur/LayerStack.h"
#include "Hildur/Events/ApplicationEvent.h"



namespace Hildur {

	class HILDUR_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

	private:

		bool OnWindoClose(WindowCloseEvent& e);


		std::unique_ptr<Window> m_Window;

		bool m_Running = true;

		LayerStack m_LayerStack;

	};

	//To be defined in CLIENT
	Application* CreateApplication();

}
