#include "DEFINITIONS.hpp"
#include "AppPlanner.hpp"
#include "SplashState.hpp"

using namespace std;

//	Constructor / Destructor
AppPlanner::AppPlanner(unsigned width, unsigned height, string title)
{
	_data->window.create(sf::VideoMode(width, height), title, sf::Style::Titlebar | sf::Style::Close);
	_data->machine.AddState(StateRef(new SplashState(this->_data)));

	this->run();
}

AppPlanner::~AppPlanner()
{

}

//	Main loop
void AppPlanner::run()
{
	float newTime;
	float frameTime;		//	time between frames (currentTime and newTime)
	float interpolation;

	float currentTime = this->_clock.getElapsedTime().asSeconds();
	float accumulator = 0.f;

	while (this->_data->window.isOpen())
	{
		this->_data->machine.ProcessStateChanges();

		newTime = this->_clock.getElapsedTime().asSeconds();
		frameTime = newTime - currentTime;
		if (frameTime > 0.25f)
		{
			frameTime = 0.25f;		//	Prevent too large frame time
		}

		currentTime = newTime;
		accumulator += frameTime;
		while (accumulator >= dt)
		{
			this->_data->machine.GetActiveState()->HandleInput();
			this->_data->machine.GetActiveState()->Update(dt);
			accumulator -= dt;
		}
		interpolation = accumulator / dt;
		this->_data->machine.GetActiveState()->Draw(interpolation);
	}
}