#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "ObjectMoveTo.hpp"
#include "Button.hpp"

using namespace std;

class StopwatchState : public GuiState
{
private:
    sf::Clock _clock;

    // Stoper
    float _elapsedTime;      // Czas w sekundach
    bool _isRunning;         // Czy stoper dzia³a

    // Grafika
    sf::Sprite _background;
    sf::Text _timeDisplay;   // Wyœwietlanie czasu

    // Przyciski
    Button _startStopButton;
    sf::Text _startStopText;
    Button _resetButton;
    sf::Text _resetText;

    // Pomocnicza funkcja do formatowania czasu
    string formatTime(float seconds);

public:
    StopwatchState(AppDataRef data);

    void State_Init();
    void State_HandleInput(sf::Event& event);
    void State_Update(float dt);
    void State_Draw(float dt);
};