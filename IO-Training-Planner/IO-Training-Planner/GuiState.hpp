#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "AppPlanner.hpp"
#include "Button.hpp"
#include "ObjectMoveTo.hpp" 

class GuiState : public State
{
protected:
    AppDataRef _data;

private:
    //	GUI elements
    sf::Vector2f _guiBarDestination;
    sf::Sprite _guiBar;
    sf::Vector2f _menuButtonDestination;
    Button _menuButton;
    sf::Vector2f _homeButtonDestination;
    Button _homeButton;
    sf::Vector2f _trainingButtonDestination;
    Button _trainingButton;
    sf::Vector2f _planButtonDestination;
    Button _planButton;
    sf::Vector2f _stopwatchButtonDestination;
    Button _stopwatchButton;
    sf::Vector2f _calculatorButtonDestination;
    Button _calculatorButton;
    sf::Vector2f _progressButtonDestination;
    Button _progressButton;
    sf::Vector2f _exerciseListButtonDestination;
    Button _exerciseListButton;

    void GUI_Init();
    void GUI_HandleInput();
    void GUI_Update(float dt);
    void GUI_Draw(float dt);

public:
    GuiState(AppDataRef data);

    virtual void Init() override;
    virtual void HandleInput() override;
    virtual void Update(float dt) override;
    virtual void Draw(float dt) override;

    virtual void State_Init() = 0;
    virtual void State_HandleInput() = 0;
    virtual void State_Update(float dt) = 0;
    virtual void State_Draw(float dt) = 0;
};