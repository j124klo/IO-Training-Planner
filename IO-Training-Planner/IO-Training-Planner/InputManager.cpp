#include "InputManager.hpp"

using namespace std;

bool InputManager::isSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window)
{
	if (sf::Mouse::isButtonPressed(button))
	{
		sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);

		updateMousePositions(window);
		if (tempRect.contains(this->mousePositionWindow))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::isRectangleShapeClicked(sf::RectangleShape object, sf::Mouse::Button button, sf::RenderWindow& window)
{
	if (sf::Mouse::isButtonPressed(button))
	{
		sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);

		updateMousePositions(window);
		if (tempRect.contains(this->mousePositionWindow))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::isSpriteClickedAndReleased(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window, bool& isPressed, bool& doUpdate)
{
	if (isPressed)
	{
		if (sf::Mouse::isButtonPressed(button))
		{
			sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);

			updateMousePositions(window);
			if (tempRect.contains(this->mousePositionWindow))
			{
				return false;
			}
			else
			{
				isPressed = false;
				doUpdate = true;
				return false;
			}
		}
		else
		{
			sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);

			updateMousePositions(window);
			if (tempRect.contains(this->mousePositionWindow)) {
				isPressed = false;
				doUpdate = true;
				return true;
			}
			else
			{
				isPressed = false;
				doUpdate = true;
				return false;
			}
		}
	}
	else
	{
		if (sf::Mouse::isButtonPressed(button))
		{
			sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);

			updateMousePositions(window);
			if (tempRect.contains(this->mousePositionWindow))
			{
				isPressed = true;
				doUpdate = true;
				return false;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

bool InputManager::isKeyPressedAndReleased(sf::Keyboard::Key key, bool& isPressed, bool& doUpdate)
{
	if (isPressed)
	{
		if (sf::Keyboard::isKeyPressed(key))
		{
			return false;
		}
		else
		{
			isPressed = false;
			doUpdate = true;
			return true;
		}
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(key))
		{
			isPressed = true;
			doUpdate = true;
			return false;
		}
		else
		{
			return false;
		}
	}
}

bool InputManager::typingText(sf::Event& event, string& text, sf::Text& displayedText, bool& isActive)
{
	if (event.type == sf::Event::TextEntered)
	{
		if (isActive)
		{
			//	Backspace (Unicode 8)
			if (event.text.unicode == 8)
			{
				if (!text.empty())
					text.pop_back();
			}
			//	ASCII (Unicode 32 - 126)
			else if (event.text.unicode < 128)
			{
				text += static_cast<char>(event.text.unicode);
			}

			// Aktualizacja obiektu sf::Text
			displayedText.setString(text);
			return true;
		}
		else	return false;
	}
}

void InputManager::updateMousePositions(sf::RenderWindow& window)
{
	this->mousePositionWindow = sf::Mouse::getPosition(window);
	this->mousePositionView = window.mapPixelToCoords(this->mousePositionWindow);
}