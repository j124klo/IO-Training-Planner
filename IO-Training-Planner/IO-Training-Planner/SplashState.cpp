#include "DEFINITIONS.hpp"
#include "SplashState.hpp"
#include "HomeState.hpp"

using namespace std;

SplashState::SplashState(AppDataRef data) : _data(data)
{

}

void SplashState::Init()
{
	this->_data->assets.loadTexture("Splash State Background", SPLASH_STATE_BACKGROUND_FILEPATH);
	_background.setTexture(this->_data->assets.getTexture("Splash State Background"));
}

void SplashState::HandleInput()
{
	sf::Event event;
	while (this->_data->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			this->_data->window.close();
		}
	}
}

void SplashState::Update(float dt)
{
	if (this->_clock.getElapsedTime().asSeconds() > SPLASH_STATE_SHOW_TIME)
	{
		this->_data->machine.AddState(StateRef(new HomeState(_data)), true);
	}
}

void SplashState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::Black);
	this->_data->window.draw(this->_background);
	this->_data->window.display();
}