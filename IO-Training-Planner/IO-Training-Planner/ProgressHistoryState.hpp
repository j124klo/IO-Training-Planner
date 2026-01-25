#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "ObjectMoveTo.hpp"
#include "Button.hpp"

using namespace std;

class ProgressHistoryState : public GuiState
{
private:
	sf::Clock _clock;

	sf::Sprite _background;

	Button _backToProgressButton;

public:
	ProgressHistoryState(AppDataRef data);

	void ProgressButton_Init();
	void ProgressButton_HandleInput(sf::Event& event);
	void ProgressButton_Update(float dt);
	void ProgressButton_Draw(float dt);

	void State_Init();
	void State_HandleInput(sf::Event& event);
	void State_Update(float dt);
	void State_Draw(float dt);
};