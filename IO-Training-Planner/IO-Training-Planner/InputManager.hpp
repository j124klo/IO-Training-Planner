#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

class InputManager
{
private:
	//	Mouse
	sf::Vector2i mousePositionWindow;	//	intiger
	sf::Vector2f mousePositionView;		//	float

public:
	InputManager() {}
	~InputManager() {}

	bool isSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window);
	bool isRectangleShapeClicked(sf::RectangleShape object, sf::Mouse::Button button, sf::RenderWindow& window);
	bool isSpriteClickedAndReleased(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window, bool& isPressed, bool& doUpdate);
	bool isKeyPressedAndReleased(sf::Keyboard::Key key, bool& isPressed, bool& doUpdate);
	bool typingText(sf::Event& event, string& text, sf::Text& displayedText, bool& isActive);

	void updateMousePositions(sf::RenderWindow& window);

	inline sf::Vector2i getMousePositionWindow() const { return this->mousePositionWindow; }
	inline sf::Vector2f getMousePositionView() const { return this->mousePositionView; }
};