#include "hrpcheaders.h"
#include "WindowsWindow.h"

#include "Hildur/Events/ApplicationEvent.h"
#include "Hildur/Events/MouseEvent.h"
#include "Hildur/Events/KeyEvent.h"

#include "Hildur/Core/System/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Hildur {


	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		HR_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Scope<Window> Window::Create(const WindowProps& props) 
	{
		return CreateScope<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) 
	{
		HR_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow() 
	{
		HR_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) 
	{
		HR_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.IsFullscreen = props.IsFullscreen;

		HR_CORE_INFO("Creating window {0} ({1}, {2}, fullscreen: {3}), at ({4}, {5})", props.Title, props.Width, props.Height, props.IsFullscreen, props.PosX, props.PosY);

		if (!s_GLFWInitialized) 
		{
			HR_PROFILE_SCOPE("glfwInit")

			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			HR_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		{
			HR_PROFILE_SCOPE("glfwCreateWindow")

#if 0
#ifdef HR_DEBUG
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
#endif

			GLFWmonitor* monitor = props.IsFullscreen ? glfwGetPrimaryMonitor() : nullptr;
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), monitor, nullptr);
			//glfwSetWindowPos(m_Window, m_Data.PosX, m_Data.PosY); // TODO: Fix!!!!!!!!!!!!!
		}

		m_Context = CreateScope<OpenGLContext>(m_Window);
		m_Context->Init();

		glfwGetWindowPos(m_Window, &m_Data.PosX, &m_Data.PosY);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.PosX = xpos;
				data.PosY = ypos;

				WindowMoveEvent event(xpos, ypos);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown() 
	{
		HR_PROFILE_FUNCTION();
		
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate() 
	{
		HR_PROFILE_FUNCTION();
		
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetName(std::string& name)
	{
		m_Data.Title = name;

		glfwSetWindowTitle(m_Window, m_Data.Title.c_str());
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		HR_PROFILE_FUNCTION();
		
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const 
	{
		return m_Data.VSync;
	}


}