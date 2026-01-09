#pragma once
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

struct ExerciseItem
{
	string _name;
	string _description;
	sf::Text _nameText;
	sf::Text _descriptionText;
};