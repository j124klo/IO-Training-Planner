#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "Button.hpp"

class RepPercentageState : public GuiState
{
private:
	sf::Sprite _background;

	double _oneRM;

	sf::Text _titleLabel;
	std::vector<sf::Text> _percentageLabels;
	std::vector<sf::Text> _weightValues;

	Button _backButton;

	void InitTitle();
	void InitPercentageTable();
	void InitButtons();

public:
	RepPercentageState(AppDataRef data, double oneRM);

	void State_Init();
	void State_HandleInput(sf::Event& event);
	void State_Update(float dt);
	void State_Draw(float dt);
};
