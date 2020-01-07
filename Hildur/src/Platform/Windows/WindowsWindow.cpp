#include "hrpcheaders.h"
#include "WindowsWindow.h"

#include "Hildur/Events/ApplicationEvent.h"
#include "Hildur/Events/MouseEvent.h"
#include "Hildur/Events/KeyEvent.h"


namespace Hildur {


	static bool s_SFMLInitialized = false;

	//sf::Event event;
	int verticalScroll = 0, horizontalScroll = 0;

	int lastKeyCode = 0, isTheSame = 0;



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

		//Set SFML callbacks, though they might be automatic

		

	}

	void WindowsWindow::Shutdown() {

		HR_CORE_INFO("Y did u doo dis 2 me D':");

		m_Window.close();

	}

	void WindowsWindow::OnUpdate() {

		ProcessEvents();

	}

	void WindowsWindow::SetVSync(bool enabled) {

		m_Window.setVerticalSyncEnabled(enabled);
		m_Data.VSync = enabled;

	}

	bool WindowsWindow::IsVSync() const {

		return m_Data.VSync;

	}

	void WindowsWindow::ProcessEvents()
	{
		sf::Event Event;
		sf::Window* window;
		

		while (m_Window.pollEvent(Event))
		{
			
			//Window closed
			if (Event.type == sf::Event::Closed) {

				WindowCloseEvent event;
				m_Data.EventCallback(event);

				//Shutdown();

			}

			//Window resized
			if (Event.type == sf::Event::Resized) {


				//WindowData& data = *(WindowData*)GetWindow();
				WindowResizeEvent event(Event.size.width, Event.size.height);
				m_Data.EventCallback(event);
				

			}

			//Key Events
			if (Event.type == sf::Event::KeyPressed) {


				//WindowData& data = *(WindowData*)GetWindow(window);
				KeyPressedEvent event(Event.key.code, isTheSame);
				m_Data.EventCallback(event);
			
				isTheSame = 1;

			}

			if (Event.type == sf::Event::KeyReleased) {

				KeyReleasedEvent event(Event.key.code);
				m_Data.EventCallback(event);

				isTheSame = 0;

			}

			//Mouse Events
			if (Event.type == sf::Event::MouseButtonPressed) {


				//WindowData& data = *(WindowData*)GetWindow(window);
				MouseButtonPressedEvent event(Event.key.code);
				m_Data.EventCallback(event);


			}

			if (Event.type == sf::Event::MouseButtonReleased) {


				//WindowData& data = *(WindowData*)GetWindow(window);
				MouseButtonReleasedEvent event(Event.key.code);
				m_Data.EventCallback(event);


			}

			if (Event.type == sf::Event::MouseWheelScrolled) {

				
				if (Event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					verticalScroll = Event.mouseWheelScroll.delta;
					horizontalScroll = 0;
				}
				if (Event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
					horizontalScroll = Event.mouseWheelScroll.delta;
					verticalScroll = 0;
				}

				//WindowData& data = *(WindowData*)GetWindow(window);
				MouseScrolledEvent event(horizontalScroll, verticalScroll);
				m_Data.EventCallback(event);


			}

			if (Event.type == sf::Event::MouseMoved) {


				//WindowData& data = *(WindowData*)GetWindow(window);
				MouseMovedEvent event(Event.mouseMove.x, Event.mouseMove.y);
				m_Data.EventCallback(event);


			}
			
		}
	}

	void* WindowsWindow::GetWindow(/*sf::Window* window*/) {

		//window = &m_Window;

		return &m_Window;

	}


}