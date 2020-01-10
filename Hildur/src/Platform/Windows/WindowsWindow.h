#pragma once

#include "Hildur/Window.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace Hildur {


	class WindowsWindow : public Window {

	public:

		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate();

		inline unsigned int GetWidth() const override { return m_Data.Width; };
		inline unsigned int GetHeight() const override { return m_Data.Height; };

		//Atributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	

		inline virtual void* GetNativeWindow() const { return m_Window; }

	private:

		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		void ProcessEvents();

	private:

		struct WindowData {

			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;

			EventCallbackFn EventCallback;

		};

		WindowData m_Data;

		sf::RenderWindow* m_Window;

		sf::RenderWindow m_NewWindow;

	};


}