#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "Models.hpp"

using namespace std;

class ProgressState : public GuiState
{
private:
    sf::Sprite _background;

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

    void State_Init();
    void State_HandleInput(sf::Event& event);
    void State_Update(float dt);
    void State_Draw(float dt);
};