#include "hrpcheaders.h"
#include "Application.h"

#include "Hildur/Events/ApplicationEvent.h"


namespace Hildur {

	Application::Application() {

		//m_Window = Window::Create();
		m_Window = std::unique_ptr<Window>(Window::Create());

	}

	Application::~Application() {



	}

	void Application::Run() {

		while (true) {

			m_Window->OnUpdate();

		}

	}

}