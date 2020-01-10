#include "hrpcheaders.h"
#include "WindowsWindow.h"

#include "Hildur/Events/ApplicationEvent.h"
#include "Hildur/Events/MouseEvent.h"
#include "Hildur/Events/KeyEvent.h"

#include <glad/glad.h>

#include <imgui.h>
#include <imgui-SFML.h>


namespace Hildur {


	static bool s_SFMLInitialized = false;

	int verticalScroll = 0, horizontalScroll = 0;
	int lastKeyCode = 0, isTheSame = 0;

	short width, height;

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

		m_Window = &m_NewWindow;
		m_Window->create(sf::VideoMode(props.Width, props.Height), props.Title);

		SetVSync(true);

		width = props.Width;
		height = props.Height;

		int status = gladLoadGLLoader((GLADloadproc)wglGetProcAddress);
		HR_CORE_ASSERT(status, "Falied to initialize Glad!");

	}

	void WindowsWindow::Shutdown() {

		HR_CORE_INFO("Y did u doo dis 2 me D':");

		(*m_Window).close();

	}

	void WindowsWindow::OnUpdate() {

		ProcessEvents();

		width = m_Window->getSize().x;
		height = m_Window->getSize().y;
	}

	void WindowsWindow::SetVSync(bool enabled) {

		(*m_Window).setVerticalSyncEnabled(enabled);
		m_Data.VSync = enabled;

	}

	bool WindowsWindow::IsVSync() const {

		return m_Data.VSync;

	}

	void WindowsWindow::ProcessEvents()
	{
		sf::Event Event;
		sf::Window* window;
		
		
		while ((*m_Window).pollEvent(Event))
		{
			
			//Window closed
			if (Event.type == sf::Event::Closed) {

				WindowCloseEvent event;
				m_Data.EventCallback(event);

			}

			//Window resized
			if (Event.type == sf::Event::Resized) {

				WindowResizeEvent event(Event.size.width, Event.size.height);
				m_Data.EventCallback(event);
				
			}

			//Key Events
			if (Event.type == sf::Event::KeyPressed) {

				KeyPressedEvent event(Event.key.code, isTheSame);
				m_Data.EventCallback(event);
			
				isTheSame = 1;

			}

			if (Event.type == sf::Event::KeyReleased) {

				KeyReleasedEvent event(Event.key.code);
				m_Data.EventCallback(event);

				isTheSame = 0;

			}

			if (Event.type == sf::Event::TextEntered) {

				KeyTypedEvent event(Event.text.unicode);
				m_Data.EventCallback(event);

			}

			//Mouse Events
			if (Event.type == sf::Event::MouseButtonPressed) {

				MouseButtonPressedEvent event(Event.key.code);
				m_Data.EventCallback(event);

			}

			if (Event.type == sf::Event::MouseButtonReleased) {

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

				MouseScrolledEvent event(horizontalScroll, verticalScroll);
				m_Data.EventCallback(event);


			}

			if (Event.type == sf::Event::MouseMoved) {

				MouseMovedEvent event(Event.mouseMove.x, Event.mouseMove.y);
				m_Data.EventCallback(event);

			}

		}
		

	}


}