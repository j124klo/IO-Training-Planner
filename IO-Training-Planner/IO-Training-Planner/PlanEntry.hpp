#pragma once
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

struct PlanGUIEntry {
    int _dayId;       
    int _orderIndex; 

    string _dayName;       // "Monday"
    string _exerciseName;  // "Running"
    string _targetInfo;    // "10 km"

    sf::Text _dayText;
    sf::Text _nameText;
    sf::Text _targetText;
};