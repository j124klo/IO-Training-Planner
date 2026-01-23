#pragma once
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

struct PlanGUIEntry {
    string _dayName;
	//sf::Text _dayNameText;
    string _exerciseName;
	//sf::Text _exerciseNameText;
    string _targetInfo;
    int _dayId;
};