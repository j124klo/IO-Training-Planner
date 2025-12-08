#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "AppPlanner.hpp"

using namespace std;

class SplashState : public State
{
private:
	AppDataRef _data;
	sf::Clock _clock;
	sf::Sprite _background;

public:
	SplashState(AppDataRef data);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
};