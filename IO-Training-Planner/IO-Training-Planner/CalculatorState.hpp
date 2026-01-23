#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "ObjectMoveTo.hpp"
#include "Button.hpp"

using namespace std;

class CalculatorState : public GuiState
{
private:
	sf::Clock _clock;

	sf::Sprite _background;

public:
	CalculatorState(AppDataRef data);

	void State_Init();
	void State_HandleInput();
	void State_Update(float dt);
	void State_Draw(float dt);
};