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

    // ZMIANA: 3 osobne kontenery na dane
    vector<PersonalRecord> _weightRecords;
    vector<PersonalRecord> _repsRecords;
    vector<PersonalRecord> _timeRecords;
    vector<PersonalRecord> _distRecords;

    sf::Text _mainTitle;

    // Wszystkie teksty (nag³ówki i wiersze) trzymamy tu
    vector<sf::Text> _allTexts;

public:
    ProgressState(AppDataRef data);

    void State_Init();
    void State_HandleInput(sf::Event& event);
    void State_Update(float dt);
    void State_Draw(float dt);
};