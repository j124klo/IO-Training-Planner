#include "DEFINITIONS.hpp"
#include "TrainingState.hpp"

using namespace std;

TrainingState::TrainingState(AppDataRef data) : GuiState(data)
{

}

void TrainingState::State_Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Default Background"));
}
void TrainingState::State_HandleInput(sf::Event& event)
{

}
void TrainingState::State_Update(float dt)
{

}
void TrainingState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);
}