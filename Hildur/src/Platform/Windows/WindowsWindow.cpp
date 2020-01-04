#include "hrpcheaders.h"
#include "WindowsWindow.h"


namespace Hildur {


	static bool s_SFMLInitialized = false;

	sf::Event event;


	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {

		Init(props);

	}

	WindowsWindow::~WindowsWindow() {

		Shutdown();

	}

	void WindowsWindow::Init(const WindowProps& props) {

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HR_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);


		m_Window.create(sf::VideoMode(props.Width, props.Height), props.Title);

		SetVSync(true);

	}

	void WindowsWindow::Shutdown() {

		

	}

	void WindowsWindow::OnUpdate() {

		while (m_Window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
			case sf::Event::Closed:
				m_Window.close();
				break;


			default:
				break;
			}
		}

	}

	void WindowsWindow::SetVSync(bool enabled) {

		m_Window.setVerticalSyncEnabled(enabled);
		m_Data.VSync = enabled;

	}

	bool WindowsWindow::IsVSync() const {

		return m_Data.VSync;

	}


}