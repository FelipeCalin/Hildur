#include "hrpcheaders.h"
#include "imGuiLayer.h"

#include <SFML/Window.hpp>
#include "Platform/Windows/WindowsWindow.h"

#include "imgui-SFML.h"
#include "imgui.h"

#define IMGUI_IMPL_API
#include "examples/imgui_impl_opengl3.h"

#include "Hildur/Application.h"

#include <glad/glad.h>



namespace Hildur {

	sf::Clock deltaClock;
	sf::Color bgColor;
	
	
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer") {

		
	}

	ImGuiLayer::~ImGuiLayer() {


	}

	void ImGuiLayer::OnAttach() {

		sf::RenderWindow& m_Window = GetReference();

		ImGui::SFML::Init(m_Window);
		//bool GLStatus = ImGui_ImplOpenGL3_Init();
		//HR_ASSERT(GLStatus, "Couldn't initialize Imgui OpenGL :(")

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

	}

	void ImGuiLayer::OnDetach() {

		ImGui::SFML::Shutdown();

	}

	void ImGuiLayer::Begin() {

		ImGuiIO& io = ImGui::GetIO();
		sf::RenderWindow& m_Window = GetReference();
		

		/*sf::Event event;
		while (m_Window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

		}*/

		//ImGui::SFML::Update(m_Window, deltaClock.restart());

		//ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();


	}

	void ImGuiLayer::End() {

		ImGuiIO& io = ImGui::GetIO();
		sf::RenderWindow& m_Window = GetReference();

		//ImGui::EndFrame();
		io.DisplaySize = m_Window.getSize();
	

		//Rendering
		m_Window.clear(bgColor); // fill background with color
		ImGui::SFML::Render(m_Window);
		m_Window.display();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

			//sf::Window* backup_current_context = sf::Context
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

		}
		

	}

	void ImGuiLayer::OnImGuiRender() {

		sf::RenderWindow& m_Window = GetReference();
		ImGuiIO& io = ImGui::GetIO();


		static bool ShowDemo = true;
		ImGui::ShowDemoWindow(&ShowDemo);

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