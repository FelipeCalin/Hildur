#include "hrpcheaders.h"
#include "imGuiLayer.h"

#include "imgui.h"
#include "imgui-SFML.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "Hildur/Application.h"
#include "Platform/Windows/WindowsWindow.h"

#include <SFML/Window.hpp>


namespace Hildur {

	sf::Clock clock;
	char windowTitle[255] = "ImGui + SFML = <3";
	sf::Clock deltaClock;
	sf::Color bgColor;
	float color[3] = { 0.f, 0.f, 0.f };

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer") {

		
	}

	ImGuiLayer::~ImGuiLayer() {



	}

	void ImGuiLayer::OnAttach() {

		sf::RenderWindow& m_Window = GetReference();

		ImGui::SFML::Init(m_Window);
		m_Window.resetGLStates();
		
		m_Window.setTitle(windowTitle);
		m_Window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
		
	}

	void ImGuiLayer::OnDettach() {

		ImGui::SFML::Shutdown();

	}

	void ImGuiLayer::OnUpdate() {

		sf::RenderWindow& m_Window = GetReference();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		ImGui::SFML::Update(m_Window, deltaClock.restart());

		//WINDOW

		//ImGui::Begin("Sample window"); // begin window

		////ImGui_ImplOpenGL3_NewFrame();
		////ImGui::NewFrame();

		//if (ImGui::ColorEdit3("Background color", color)) {
		//	// this code gets called if color value changes, so
		//	// the background color is upgraded automatically!
		//	bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
		//	bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
		//	bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
		//}

		//ImGui::InputText("Window title", windowTitle, 255);

		//if (ImGui::Button("Update window title")) {
		//	// this code gets if user clicks on the button
		//	// yes, you could have written if(ImGui::InputText(...))
		//	// but I do this to show how buttons work :)
		//	m_Window.setTitle(windowTitle);
		//}
		//ImGui::End(); // end window

		//ImGui::EndFrame();

		////TEST
		////----------------------------------

		//ImGui::NewFrame();

		static bool ShowDemo = true;
		ImGui::ShowDemoWindow(&ShowDemo);

        //----------------------------------

		m_Window.clear(bgColor); // fill background with color
		ImGui::SFML::Render(m_Window);
		m_Window.display();
    }


	void ImGuiLayer::OnEvent(Event& event) {

		sf::RenderWindow& m_Window = GetReference();

		EventDispatcher distpatcher(event);
		distpatcher.Dispatch<MouseButtonPressedEvent>(HR_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		distpatcher.Dispatch<MouseButtonReleasedEvent>(HR_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		distpatcher.Dispatch<MouseScrolledEvent>(HR_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		distpatcher.Dispatch<MouseMovedEvent>(HR_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		distpatcher.Dispatch<KeyPressedEvent>(HR_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		distpatcher.Dispatch<KeyReleasedEvent>(HR_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		//distpatcher.Dispatch<OnKeyTypedEvent>(HR_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		distpatcher.Dispatch<WindowResizeEvent>(HR_BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent));

		sf::Event wevent;
		while (m_Window.pollEvent(wevent)) {
			ImGui::SFML::ProcessEvent(wevent);

			if (wevent.type == sf::Event::Closed) {
				m_Window.close();
			}
		}

	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent& e)
	{
		return false;
	}

	sf::RenderWindow& ImGuiLayer::GetReference()
	{
		
		Application& app = Application::Get();
		sf::RenderWindow* m_Window = static_cast<sf::RenderWindow*>(app.GetWindow().GetNativeWindow());

		sf::RenderWindow& m_NewWindow = *m_Window;

		return m_NewWindow;
	}


}