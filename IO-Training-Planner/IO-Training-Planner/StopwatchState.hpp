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

enum class IntervalPhase
{
    Prepare,    // Preparation before start
    Work,       // Work phase
    Rest,       // Rest phase
    Finished    // Completed
};

class StopwatchState : public GuiState
{
private:
    sf::Clock _clock;

    // Interval state
    float _currentTime;
    bool _isRunning;
    IntervalPhase _phase;

    // Interval settings
    int _workSeconds;       // Work time (seconds)
    int _restSeconds;       // Rest time (seconds)
    int _prepareSeconds;    // Preparation time
    int _totalRounds;       // Total rounds
    int _currentRound;      // Current round

    // Graphics
    sf::Sprite _background;
    sf::Text _timeDisplay;
    sf::Text _phaseLabel;
    sf::Text _roundLabel;

    // Main buttons
    sf::Text _startStopText;
    sf::Text _resetText;

    // Settings - work
    sf::Text _workLabel;
    sf::Text _workTimeText;
    sf::Text _workPlusText;
    sf::Text _workMinusText;

    // Settings - rest
    sf::Text _restLabel;
    sf::Text _restTimeText;
    sf::Text _restPlusText;
    sf::Text _restMinusText;

    // Settings - rounds
    sf::Text _roundsLabel;
    sf::Text _roundsText;
    sf::Text _roundsPlusText;
    sf::Text _roundsMinusText;

    // Progress bar
    sf::RectangleShape _progressBarBg;
    sf::RectangleShape _progressBarFill;

    // Helper functions
    string formatTime(float seconds);
    string formatTimeShort(int seconds);
    void setupText(sf::Text& text, const string& label, int size, float x, float y, sf::Color color = sf::Color::White);
    void updateSettingsDisplay();
    void nextPhase();
    float getCurrentPhaseDuration();

public:
    StopwatchState(AppDataRef data);

    void State_Init();
    void State_HandleInput(sf::Event& event);
    void State_Update(float dt);
    void State_Draw(float dt);
};