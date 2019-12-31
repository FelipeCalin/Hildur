#include "hrpcheaders.h"
#include "Application.h"

#include "Hildur/Events/ApplicationEvent.h"
#include "Hildur/Log.h"


namespace Hildur {

	Application::Application() {


	}

	Application::~Application() {


	}

	void Application::Run() {

		WindowResizeEvent e(1280, 720);
		HR_TRACE(e);

		while (true);

	}

}