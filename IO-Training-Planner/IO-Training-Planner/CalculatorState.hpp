#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "ObjectMoveTo.hpp"
#include "Button.hpp"

#include <iomanip>

using namespace std;

class CalculatorState : public GuiState
{
private:
	sf::Clock _clock;

	sf::Sprite _background;

    // Input fields
    sf::Text _weightLabel;
    sf::Text _weightInput;
    sf::RectangleShape _weightBounds;
    string _weightText;
    bool _isWeightActive;

    sf::Text _repsLabel;
    sf::Text _repsInput;
    sf::RectangleShape _repsBounds;
    string _repsText;
    bool _isRepsActive;

    // Result display
    sf::Text _resultValue;

    // Calculate button
    Button _calculateButton;

public:
	CalculatorState(AppDataRef data);

    double Calculate1RM(double weight, int reps);

    void InputFields_Init();
    void Labels_Init();
    void Buttons_Init();

	void State_Init();
	void State_HandleInput(sf::Event& event);
	void State_Update(float dt);
	void State_Draw(float dt);
};