#include "hrpcheaders.h"
#include "WindowsInput.h"

#include "Hildur/Application.h"

#include <SFML/Window.hpp>


namespace Hildur {

	Input* Input::s_Instance = new WindowsInput();


	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		sf::Keyboard::Key keyboard = (sf::Keyboard::Key)keycode;
		return sf::Keyboard::isKeyPressed(keyboard);
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		sf::Mouse::Button mouse = (sf::Mouse::Button)button;
		return sf::Mouse::isButtonPressed(mouse);
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		return { sf::Mouse::getPosition().x, sf::Mouse::getPosition().y };
	}

	float WindowsInput::GetMouseXImpl()
	{
		return sf::Mouse::getPosition().x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		return sf::Mouse::getPosition().y;
	}

}