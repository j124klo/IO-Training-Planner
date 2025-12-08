#pragma once

#include <iostream>
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>
#include "StateMachine.hpp"
#include "AssetManager.hpp"
#include "InputManager.hpp"

using namespace std;

struct AppData
{
	StateMachine machine;
	sf::RenderWindow window;
	AssetManager assets;
	InputManager input;
};

typedef shared_ptr<AppData> AppDataRef;

class AppPlanner
{
private:
	//	App time
	const float dt = 1.f / 60.f;
	sf::Clock _clock;

	//	App data
	AppDataRef _data = make_shared<AppData>();

	//	Run game
	void run();

public:
	//	Constructor / Destructor
	AppPlanner(unsigned width, unsigned height, string title);
	~AppPlanner();
};