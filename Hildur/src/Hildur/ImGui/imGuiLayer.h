#pragma once

#include "Hildur/Layer.h"

#include "Hildur/Events/ApplicationEvent.h"
#include "Hildur/Events/KeyEvent.h"
#include "Hildur/Events/MouseEvent.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


namespace Hildur {


	class HILDUR_API ImGuiLayer : public Layer {

	public:

		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDettach();
		void OnUpdate();
		void OnEvent(Event& event);

		sf::RenderWindow& GetReference();

	private:

		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		//bool OnkeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizedEvent(WindowResizeEvent& e);

	private:

		float m_Time = 0.0f;

	};


}