#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "Models.hpp"

using namespace std;

class ProgressState : public GuiState
{
private:
    sf::Clock _clock;

    sf::Sprite _background;

	Button _progressHistoryButton;

    // ZMIANA: Teraz przechowujemy rekordy, a nie ca³¹ historiê
    vector<PersonalRecord> _records;

    sf::Text _titleText;

    // Zredukowane nag³ówki
    sf::Text _headerExercise;
    sf::Text _headerMaxWeight;
    sf::Text _headerMaxReps;
    sf::Text _headerDate;

    vector<sf::Text> _logTexts;

public:
    ProgressState(AppDataRef data);

	void ProgressHistoryButton_Init();
	void ProgressHistoryButton_HandleInput(sf::Event& event);
	void ProgressHistoryButton_Update(float dt);
	void ProgressHistoryButton_Draw(float dt);

    void State_Init();
    void State_HandleInput(sf::Event& event);
    void State_Update(float dt);
    void State_Draw(float dt);
};