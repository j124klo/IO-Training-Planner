#include "DEFINITIONS.hpp"
#include "ProgressState.hpp"

using namespace std;

ProgressState::ProgressState(AppDataRef data) : GuiState(data)
{

}

void ProgressState::State_Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Default Background"));
}
void ProgressState::State_HandleInput(sf::Event& event)
{

}
void ProgressState::State_Update(float dt)
{

}
void ProgressState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);
}