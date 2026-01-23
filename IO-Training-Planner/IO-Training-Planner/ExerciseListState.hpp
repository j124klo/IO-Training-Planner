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

using namespace std;

class ExerciseListState : public GuiState
{
private:
	sf::Clock _clock;

	sf::Sprite _background;

	//	Exercise entries
	vector<ExerciseItem> _exercises;

public:
	ExerciseListState(AppDataRef data);

	void Exercises_Init();
	void Exercises_Draw(float dt);

	void State_Init();
	void State_HandleInput();
	void State_Update(float dt);
	void State_Draw(float dt);
};