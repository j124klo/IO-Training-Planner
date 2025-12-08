#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "AppPlanner.hpp"

using namespace std;

class HomeState : public State
{
private:
	AppDataRef _data;
	sf::Clock _clock;

	sf::Sprite _background;
	sf::Sprite _BlackJack_station;
	sf::Sprite _gui_bar;

public:
	HomeState(AppDataRef data);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
};