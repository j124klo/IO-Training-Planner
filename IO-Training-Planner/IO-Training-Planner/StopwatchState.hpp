#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "ObjectMoveTo.hpp"
#include "Button.hpp"

using namespace std;

class StopwatchState : public GuiState
{
private:
	sf::Clock _clock;

	sf::Sprite _background;

public:
	StopwatchState(AppDataRef data);

	void State_Init();
	void State_HandleInput(sf::Event& event);
	void State_Update(float dt);
	void State_Draw(float dt);
};