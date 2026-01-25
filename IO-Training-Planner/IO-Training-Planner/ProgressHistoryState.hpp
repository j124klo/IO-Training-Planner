#pragma once

#include <iostream>
#include <vector> // <--- Dodaj to
#include <sstream>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "ObjectMoveTo.hpp"
#include "Button.hpp"
#include "Models.hpp" // <--- Dodaj to, ¿eby widzieæ WorkoutLog

using namespace std;

class ProgressHistoryState : public GuiState
{
private:
	sf::Clock _clock;
	sf::Sprite _background;
	Button _backToProgressButton;

	// --- NOWE ELEMENTY ---
	sf::Text _title;
	vector<WorkoutLog> _logs; // Kontener na historiê
	vector<sf::Text> _logTexts; // Teksty do wyœwietlenia

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