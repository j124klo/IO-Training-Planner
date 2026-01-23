#include "DEFINITIONS.hpp"
#include "StopwatchState.hpp"

using namespace std;

StopwatchState::StopwatchState(AppDataRef data) : GuiState(data)
{

}

void StopwatchState::State_Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Default Background"));
}
void StopwatchState::State_HandleInput(sf::Event& event)
{

}
void StopwatchState::State_Update(float dt)
{

}
void StopwatchState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);
}