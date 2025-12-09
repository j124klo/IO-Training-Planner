#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "AppPlanner.hpp"
#include "ObjectMoveTo.hpp"

using namespace std;

class HomeState : public State
{
private:
	AppDataRef _data;
	sf::Clock _clock;

	sf::Sprite _background;
	sf::Sprite _planPanel;
	vector<sf::Text> _planTexts;

	sf::Vector2f _guiBarDestination;
	sf::Sprite _guiBar;
	sf::Sprite _menuButton;
	sf::Sprite _homeButton;

public:
	HomeState(AppDataRef data);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
};