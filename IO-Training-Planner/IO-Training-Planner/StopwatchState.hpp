#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "ObjectMoveTo.hpp"
#include "Button.hpp"

using namespace std;

enum class StopwatchMode
{
    Stopwatch,  // Stoper (liczy w górê)
    Timer       // Timer (odlicza w dó³)
};

class StopwatchState : public GuiState
{
private:
    sf::Clock _clock;

    // Stoper
    float _elapsedTime;
    bool _isRunning;
    StopwatchMode _mode;

    // Timer - ustawienia
    float _timerDuration;
    int _timerMinutes;
    int _timerSeconds;
    bool _timerFinished;

    // Okr¹¿enia
    vector<float> _laps;
    vector<sf::Text> _lapTexts;

    // Grafika
    sf::Sprite _background;
    sf::Text _timeDisplay;
    sf::Text _modeLabel;

    // Przyciski
    Button _startStopButton;
    sf::Text _startStopText;
    Button _resetButton;
    sf::Text _resetText;
    Button _lapButton;
    sf::Text _lapText;
    Button _modeButton;
    sf::Text _modeText;

    // Timer controls
    sf::Text _timerSettingText;
    sf::Text _plusMinText;
    sf::Text _minusMinText;
    sf::Text _plusSecText;
    sf::Text _minusSecText;

    // Pomocnicze funkcje
    string formatTime(float seconds);
    void updateLapDisplay();
    void setupButton(sf::Text& text, const string& label, float x, float y);

public:
    StopwatchState(AppDataRef data);

    void State_Init();
    void State_HandleInput(sf::Event& event);
    void State_Update(float dt);
    void State_Draw(float dt);
};