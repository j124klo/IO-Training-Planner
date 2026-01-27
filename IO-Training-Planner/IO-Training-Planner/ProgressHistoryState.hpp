#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "Models.hpp"

using namespace std;

class ProgressHistoryState : public GuiState
{
private:
	sf::Clock _clock;

    sf::Sprite _background;

	Button _backButton;

	//	scroll panel
	sf::Sprite _scrollEdgeTop;
	sf::Sprite _scrollEdgeBot;
	sf::Sprite _scrollTop;
	sf::Sprite _scrollMid;
	sf::Sprite _scrollBot;
	sf::Vector2f _scrollTopDestination;

	//	logs
    sf::Text _title;
    vector<WorkoutLog> _logs;
	vector<sf::Text> _logDates;
	vector<sf::Text> _logExercises;
	vector<sf::Text> _logSets;
	vector<sf::Text> _logResults;

    // Helper do formatowania wyniku
    string formatLogResult(const WorkoutLog& log);

public:
    ProgressHistoryState(AppDataRef data);

	void Buttons_Init();
	void Buttons_HandleInput(sf::Event& event);
	void Buttons_Update(float dt);
	void Buttons_Draw(float dt);

	void Logs_Init();
	void Logs_HandleInput(sf::Event& event);
	void Logs_Update(float dt);
	void Logs_Draw(float dt);

	void ScrollPanel_Init();
	void ScrollPanel_HandleInput(sf::Event& event);
	void ScrollPanel_Update(float dt);
	void ScrollPanel_Draw(float dt);

    void State_Init();
    void State_HandleInput(sf::Event& event);
    void State_Update(float dt);
    void State_Draw(float dt);
};