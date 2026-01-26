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

	//	scroll panel
	sf::Sprite _scrollEdgeTop;
	sf::Sprite _scrollEdgeBot;
	sf::Sprite _scrollTop;
	sf::Sprite _scrollMid;
	sf::Sprite _scrollBot;
	sf::Vector2f _scrollTopDestination;

	//	Exercise entries
	vector<ExerciseItem> _exercises;

public:
	ExerciseListState(AppDataRef data);

	void Exercises_Init();
	void Exercises_Draw(float dt);

	void ScrollPanel_Init();
	void ScrollPanel_HandleInput(sf::Event& event);
	void ScrollPanel_Update(float dt);
	void ScrollPanel_Draw(float dt);

	void State_Init();
	void State_HandleInput(sf::Event& event);
	void State_Update(float dt);
	void State_Draw(float dt);
};