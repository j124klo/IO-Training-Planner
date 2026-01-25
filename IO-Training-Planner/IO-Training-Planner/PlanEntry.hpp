#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <variant>
#include <vector>
#include "Models.hpp"

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

    int _exerciseId;                    // Musimy wiedzieæ co to za æwiczenie
    vector<ResultValue> _targets;       // Surowe liczby (np. 100.0, 5)

    sf::FloatRect getGlobalBounds() const {
        return _nameText.getGlobalBounds();
    }
};