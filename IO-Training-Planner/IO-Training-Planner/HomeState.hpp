#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "ObjectMoveTo.hpp"
#include "Button.hpp"

using namespace std;

class HomeState : public GuiState
{
private:
	sf::Clock _clock;

	sf::Sprite _background;

	Button _startTrainingButton;

	sf::Sprite _planPanel;
	vector<sf::Text> _planTexts;

public:
	HomeState(AppDataRef data);

	void Buttons_Init();
	void Buttons_HandleInput(sf::Event& event);
	void Buttons_Update(float dt);
	void Buttons_Draw(float dt);

	void PlanPanel_Init();
	void PlanPanel_HandleInput(sf::Event& event);
	void PlanPanel_Update(float dt);
	void PlanPanel_Draw(float dt);

	void ScrollPanel_Init();
	void ScrollPanel_HandleInput(sf::Event& event);
	void ScrollPanel_Update(float dt);
	void ScrollPanel_Draw(float dt);

	void State_Init();
	void State_HandleInput(sf::Event& event);
	void State_Update(float dt);
	void State_Draw(float dt);
};