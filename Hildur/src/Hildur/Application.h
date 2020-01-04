#pragma once

#include "Core.h"
#include "Events/Event.h"

#include "Hildur/Window.h"


namespace Hildur {

	class HILDUR_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:

		std::unique_ptr<Window> m_Window;

	};

	//To be defined in CLIENT
	Application* CreateApplication();

}
