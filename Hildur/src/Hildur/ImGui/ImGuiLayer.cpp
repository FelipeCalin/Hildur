#include "hrpcheaders.h"
#include "imGuiLayer.h"

#include <SFML/Window.hpp>
#include "Platform/Windows/WindowsWindow.h"

#include "imgui-SFML.h"
#include "imgui.h"

#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "Hildur/Application.h"

#include <glad/glad.h>



namespace Hildur {

	sf::Clock deltaClock;
	sf::Time deltaTime;
	sf::Color bgColor;
	
	
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer") {

		
	}

	ImGuiLayer::~ImGuiLayer() {


	}

	void ImGuiLayer::OnAttach() {

		sf::RenderWindow& m_Window = GetReference();

		ImGui::SFML::Init(m_Window);

	}

	void ImGuiLayer::OnDettach() {

		ImGui::GetIO().Fonts->TexID = (ImTextureID)NULL;

	}

	void ImGuiLayer::OnUpdate() {

		sf::RenderWindow& m_Window = GetReference();
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = m_Window.getSize();

		ImGui::NewFrame();

		static bool ShowDemo = true;
		ImGui::ShowDemoWindow(&ShowDemo);

	    /*
		ImGui::NewFrame();

		ImGui::Begin("Demo window");
		ImGui::Button("Hello!");
		ImGui::Spacing();
		ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue );
		ImGui::End();
		*/


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
		distpatcher.Dispatch<KeyTypedEvent>(HR_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		distpatcher.Dispatch<WindowResizeEvent>(HR_BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent));
		
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) {

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) {

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e) {

		sf::RenderWindow& m_Window = GetReference();
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e) {

		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e) {

		sf::RenderWindow& m_Window = GetReference();
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyAlt = io.KeysDown[sf::Keyboard::LAlt] || io.KeysDown[sf::Keyboard::RAlt];
		io.KeyCtrl = io.KeysDown[sf::Keyboard::LControl] || io.KeysDown[sf::Keyboard::RControl];
		io.KeyShift = io.KeysDown[sf::Keyboard::LShift] || io.KeysDown[sf::Keyboard::RShift];
		io.KeySuper = io.KeysDown[sf::Keyboard::LSystem] || io.KeysDown[sf::Keyboard::RSystem];

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e) {

		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e) {

		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode < ' ' || keycode == 127) {
			
		}
		else {
			io.AddInputCharacter((unsigned short)keycode);
		}

		return false;
	}

	bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent& e) {

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

		return false;
	}

	sf::RenderWindow& ImGuiLayer::GetReference() {
		
		Application& app = Application::Get();
		sf::RenderWindow* m_Window = static_cast<sf::RenderWindow*>(app.GetWindow().GetNativeWindow());

		sf::RenderWindow& m_NewWindow = *m_Window;

		return m_NewWindow;
	}


}