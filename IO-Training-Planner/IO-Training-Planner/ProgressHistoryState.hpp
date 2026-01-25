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
    sf::Sprite _background;
    sf::Font _font;

    // Przycisk powrotu (Kszta³t, nie obrazek)
    sf::RectangleShape _backButtonShape;
    sf::Text _backButtonLabel;

    sf::Text _title;
    vector<WorkoutLog> _logs;
    vector<sf::Text> _logTexts;

    // Helper do formatowania wyniku
    string formatLogResult(const WorkoutLog& log);

public:
    ProgressHistoryState(AppDataRef data);

    void State_Init();
    void State_HandleInput(sf::Event& event);
    void State_Update(float dt);
    void State_Draw(float dt);
};