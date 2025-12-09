#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

struct Button
{
	//	do wy³¹czenia klikalnoœci
	bool _accessible;

	bool _pressed;

	//	do wyszarzenia przy kliknieciu
	bool _update;
	sf::IntRect _rect;

	//	faktyczna grafika przycisku
	sf::Sprite _sprite;
};