#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "ObjectMoveTo.hpp"
#include "Button.hpp"
#include "Exercise.hpp"
#include "DatabaseManager.hpp"
#include "PlanEntry.hpp"
#include <algorithm>
#include <iomanip>

using namespace std;

class PlanState : public GuiState
{
private:
	sf::Clock _clock;

	sf::Sprite _background;

	//string getDayName(int dayId);
	//vector<PlanGUIEntry> _planEntries;

public:
	PlanState(AppDataRef data);

	void Plan_Init();
	void Plan_Draw(float dt);

	void State_Init();
	void State_HandleInput(sf::Event& event);
	void State_Update(float dt);
	void State_Draw(float dt);
};