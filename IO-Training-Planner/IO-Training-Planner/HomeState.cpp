#include "DEFINITIONS.hpp"
#include "HomeState.hpp"
#include "ExerciseListState.hpp"
#include "PlanState.hpp"
//	#include pozosta³e stany do których mo¿na przejœæ z tego stanu

using namespace std;

HomeState::HomeState(AppDataRef data) : GuiState(data)
{

}

void HomeState::State_Init()
{
	this->_data->assets.loadTexture("Deafult Background", DEAFULT_BACKGROUND_FILEPATH);
	this->_data->assets.loadTexture("Plan Panel", PLAN_PANEL_FILEPATH);

	this->_background.setTexture(this->_data->assets.getTexture("Deafult Background"));
	this->_planPanel.setTexture(this->_data->assets.getTexture("Plan Panel"));

	this->_planPanel.setScale(0.6f, 0.6f);
	this->_planPanel.setPosition(
		105.0f * 2 + (SCREAN_WIDTH - 105.0f * 2 - this->_planPanel.getGlobalBounds().width) / 2,		//	temp	105.0f zamiast this->_menuButton._sprite.getGlobalBounds().width
		SCREAN_HEIGHT / 2 - this->_planPanel.getGlobalBounds().height / 2
	);
}

void HomeState::State_HandleInput()
{

}

void HomeState::State_Update(float dt)
{
	
}

void HomeState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);
	this->_data->window.draw(this->_planPanel);
}