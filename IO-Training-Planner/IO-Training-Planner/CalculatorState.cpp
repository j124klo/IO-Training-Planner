#include "DEFINITIONS.hpp"
#include "CalculatorState.hpp"

using namespace std;

CalculatorState::CalculatorState(AppDataRef data) : GuiState(data)
{

}

void CalculatorState::State_Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Default Background"));
}
void CalculatorState::State_HandleInput()
{

}
void CalculatorState::State_Update(float dt)
{

}
void CalculatorState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);
}