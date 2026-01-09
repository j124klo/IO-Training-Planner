#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "AppPlanner.hpp"
#include "ObjectMoveTo.hpp"
#include "Button.hpp"
#include "Exercise.hpp"

using namespace std;

class ExerciseListState : public State
{
private:
	AppDataRef _data;
	sf::Clock _clock;

	sf::Sprite _background;

	//	GUI elements
	sf::Vector2f _guiBarDestination;
	sf::Sprite _guiBar;
	sf::Vector2f _menuButtonDestination;
	Button _menuButton;
	sf::Vector2f _homeButtonDestination;
	Button _homeButton;
	sf::Vector2f _trainingButtonDestination;
	Button _trainingButton;
	sf::Vector2f _planButtonDestination;
	Button _planButton;
	sf::Vector2f _stopwatchButtonDestination;
	Button _stopwatchButton;
	sf::Vector2f _calculatorButtonDestination;
	Button _calculatorButton;
	sf::Vector2f _progressButtonDestination;
	Button _progressButton;
	sf::Vector2f _exerciseListButtonDestination;
	Button _exerciseListButton;

	//	Exercise entries
	vector<Exercise> _exercises;

public:
	ExerciseListState(AppDataRef data);

	void GUI_Init();
	void GUI_HandleInput();
	void GUI_Update(float dt);
	void GUI_Draw(float dt);

	void Exercises_Init();


	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
};