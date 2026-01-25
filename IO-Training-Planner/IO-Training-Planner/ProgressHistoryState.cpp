#include "DEFINITIONS.hpp"
#include "ProgressHistoryState.hpp"
#include "ProgressState.hpp"

using namespace std;

ProgressHistoryState::ProgressHistoryState(AppDataRef data) : GuiState(data)
{

}

void ProgressHistoryState::ProgressButton_Init()
{
    this->_backToProgressButton._sprite.setTexture(this->_data->assets.getTexture("Progress Button"));

    float menuButtonWidth;
    float menuButtonHeight;
    sf::Vector2f menuButtonPosition = GetMenuButtonPosition(menuButtonWidth, menuButtonHeight);

    this->_backToProgressButton._pressed = false;
    this->_backToProgressButton._update = false;
    this->_backToProgressButton._rect = sf::IntRect(0, 0, 300, 300);
    this->_backToProgressButton._sprite.setTextureRect(this->_backToProgressButton._rect);
    this->_backToProgressButton._sprite.setScale(0.35f, 0.35f);
    this->_backToProgressButton._sprite.setPosition(menuButtonPosition.x, SCREAN_HEIGHT - this->_backToProgressButton._sprite.getGlobalBounds().height - 20.0f);
}
void ProgressHistoryState::ProgressButton_HandleInput(sf::Event& event)
{
    if (this->_data->input.isSpriteClickedAndReleased(this->_backToProgressButton._sprite, sf::Mouse::Left, this->_data->window, this->_backToProgressButton._pressed, this->_backToProgressButton._update))
    {
        this->_data->machine.AddState(StateRef(new ProgressState(this->_data)), true);
    }
}
void ProgressHistoryState::ProgressButton_Update(float dt)
{
    if (this->_backToProgressButton._update)
    {
        this->_backToProgressButton._update = false;

        if (this->_backToProgressButton._pressed)
        {
            this->_backToProgressButton._rect.left = 300;
            this->_backToProgressButton._sprite.setTextureRect(this->_backToProgressButton._rect);
        }
        else
        {
            this->_backToProgressButton._rect.left = 0;
            this->_backToProgressButton._sprite.setTextureRect(this->_backToProgressButton._rect);
        }
    }
}
void ProgressHistoryState::ProgressButton_Draw(float dt)
{
    this->_data->window.draw(this->_backToProgressButton._sprite);
}

void ProgressHistoryState::State_Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Default Background"));

    ProgressButton_Init();
}
void ProgressHistoryState::State_HandleInput(sf::Event& event)
{
	ProgressButton_HandleInput(event);
}
void ProgressHistoryState::State_Update(float dt)
{
	ProgressButton_Update(dt);
}
void ProgressHistoryState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);

	ProgressButton_Draw(dt);
}