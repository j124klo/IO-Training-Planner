#include "DEFINITIONS.hpp"
#include "ExerciseListState.hpp"
#include "HomeState.hpp"

using namespace std;

ExerciseListState::ExerciseListState(AppDataRef data) : _data(data)
{

}

void ExerciseListState::GUI_Init()
{
	this->_guiBar.setTexture(this->_data->assets.getTexture("Gui Bar"));
	this->_menuButton._sprite.setTexture(this->_data->assets.getTexture("Menu Button"));
	this->_homeButton._sprite.setTexture(this->_data->assets.getTexture("Home Button"));
	this->_trainingButton._sprite.setTexture(this->_data->assets.getTexture("Training Button"));
	this->_planButton._sprite.setTexture(this->_data->assets.getTexture("Plan Button"));
	this->_stopwatchButton._sprite.setTexture(this->_data->assets.getTexture("Stopwatch Button"));
	this->_calculatorButton._sprite.setTexture(this->_data->assets.getTexture("Calculator Button"));
	this->_progressButton._sprite.setTexture(this->_data->assets.getTexture("Progress Button"));
	this->_exerciseListButton._sprite.setTexture(this->_data->assets.getTexture("Exercise List Button"));



	this->_guiBarDestination.x = 0 - this->_guiBar.getGlobalBounds().width * 0.9;
	this->_guiBarDestination.y = 0.0f;
	this->_guiBar.setPosition(0 - this->_guiBar.getGlobalBounds().width * 0.9, 0);


	//	MENU
	this->_menuButton._pressed = false;
	this->_menuButton._update = false;
	this->_menuButton._rect = sf::IntRect(0, 0, 300, 300);
	this->_menuButton._sprite.setTextureRect(this->_menuButton._rect);
	this->_menuButton._sprite.setScale(0.35f, 0.35f);
	this->_menuButtonDestination.x = this->_menuButton._sprite.getGlobalBounds().width;
	this->_menuButtonDestination.y = 20.0f;
	this->_menuButton._sprite.setPosition(this->_menuButton._sprite.getGlobalBounds().width, 20.0f);

	//	HOME
	this->_homeButton._pressed = false;
	this->_homeButton._update = false;
	this->_homeButton._rect = sf::IntRect(0, 0, 300, 300);
	this->_homeButton._sprite.setTextureRect(this->_homeButton._rect);
	this->_homeButton._sprite.setScale(0.35f, 0.35f);
	this->_homeButtonDestination.x = this->_homeButton._sprite.getGlobalBounds().width;
	this->_homeButtonDestination.y = 20.0f + (this->_homeButton._sprite.getGlobalBounds().height + 40.0f);
	this->_homeButton._sprite.setPosition(
		this->_homeButton._sprite.getGlobalBounds().width,
		20.0f + (this->_homeButton._sprite.getGlobalBounds().height + 40.0f)
	);


	//	TRAINING
	this->_trainingButton._pressed = false;
	this->_trainingButton._update = false;
	this->_trainingButton._rect = sf::IntRect(0, 0, 300, 300);
	this->_trainingButton._sprite.setTextureRect(this->_trainingButton._rect);
	this->_trainingButton._sprite.setScale(0.35f, 0.35f);
	this->_trainingButtonDestination.x = 0 - this->_guiBar.getGlobalBounds().width / 3 - this->_trainingButton._sprite.getGlobalBounds().width / 2 + 10.0f;
	this->_trainingButtonDestination.y = 20.0f + (this->_trainingButton._sprite.getGlobalBounds().height + 40.0f);
	this->_trainingButton._sprite.setPosition(
		0 - this->_guiBar.getGlobalBounds().width / 3 - this->_trainingButton._sprite.getGlobalBounds().width / 2 + 10.0f,
		20.0f + (this->_trainingButton._sprite.getGlobalBounds().height + 40.0f)
	);

	//	PLAN
	this->_planButton._pressed = false;
	this->_planButton._update = false;
	this->_planButton._rect = sf::IntRect(0, 0, 300, 300);
	this->_planButton._sprite.setTextureRect(this->_planButton._rect);
	this->_planButton._sprite.setScale(0.35f, 0.35f);
	this->_planButtonDestination.x = 0 - this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_planButton._sprite.getGlobalBounds().width / 2 - 10.0f;
	this->_planButtonDestination.y = 20.0f + (this->_planButton._sprite.getGlobalBounds().height + 40.0f);
	this->_planButton._sprite.setPosition(
		0 - this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_planButton._sprite.getGlobalBounds().width / 2 - 10.0f,
		20.0f + (this->_planButton._sprite.getGlobalBounds().height + 40.0f)
	);


	//	STOPWATCH
	this->_stopwatchButton._pressed = false;
	this->_stopwatchButton._update = false;
	this->_stopwatchButton._rect = sf::IntRect(0, 0, 300, 300);
	this->_stopwatchButton._sprite.setTextureRect(this->_stopwatchButton._rect);
	this->_stopwatchButton._sprite.setScale(0.35f, 0.35f);
	this->_stopwatchButtonDestination.x = 0 - this->_guiBar.getGlobalBounds().width / 3 - this->_stopwatchButton._sprite.getGlobalBounds().width / 2 + 10.0f;
	this->_stopwatchButtonDestination.y = 20.0f + 2 * (this->_stopwatchButton._sprite.getGlobalBounds().height + 40.0f);
	this->_stopwatchButton._sprite.setPosition(
		0 - this->_guiBar.getGlobalBounds().width / 3 - this->_stopwatchButton._sprite.getGlobalBounds().width / 2 + 10.0f,
		20.0f + 2 * (this->_stopwatchButton._sprite.getGlobalBounds().height + 40.0f)
	);

	//	CALCULATOR
	this->_calculatorButton._pressed = false;
	this->_calculatorButton._update = false;
	this->_calculatorButton._rect = sf::IntRect(0, 0, 300, 300);
	this->_calculatorButton._sprite.setTextureRect(this->_calculatorButton._rect);
	this->_calculatorButton._sprite.setScale(0.35f, 0.35f);
	this->_calculatorButtonDestination.x = 0 - this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_calculatorButton._sprite.getGlobalBounds().width / 2 - 10.0f;
	this->_calculatorButtonDestination.y = 20.0f + 2 * (this->_calculatorButton._sprite.getGlobalBounds().height + 40.0f);
	this->_calculatorButton._sprite.setPosition(
		0 - this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_calculatorButton._sprite.getGlobalBounds().width / 2 - 10.0f,
		20.0f + 2 * (this->_calculatorButton._sprite.getGlobalBounds().height + 40.0f)
	);


	//	PROGRESS
	this->_progressButton._pressed = false;
	this->_progressButton._update = false;
	this->_progressButton._rect = sf::IntRect(0, 0, 300, 300);
	this->_progressButton._sprite.setTextureRect(this->_progressButton._rect);
	this->_progressButton._sprite.setScale(0.35f, 0.35f);
	this->_progressButtonDestination.x = 0 - this->_guiBar.getGlobalBounds().width / 3 - this->_progressButton._sprite.getGlobalBounds().width / 2 + 10.0f;
	this->_progressButtonDestination.y = 20.0f + 3 * (this->_progressButton._sprite.getGlobalBounds().height + 40.0f);
	this->_progressButton._sprite.setPosition(
		0 - this->_guiBar.getGlobalBounds().width / 3 - this->_progressButton._sprite.getGlobalBounds().width / 2 + 10.0f,
		20.0f + 3 * (this->_progressButton._sprite.getGlobalBounds().height + 40.0f)
	);

	//	EXERCISE LIST
	this->_exerciseListButton._pressed = false;
	this->_exerciseListButton._update = false;
	this->_exerciseListButton._rect = sf::IntRect(0, 0, 300, 300);
	this->_exerciseListButton._sprite.setTextureRect(this->_exerciseListButton._rect);
	this->_exerciseListButton._sprite.setScale(0.35f, 0.35f);
	this->_exerciseListButtonDestination.x = 0 - this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_exerciseListButton._sprite.getGlobalBounds().width / 2 - 10.0f;
	this->_exerciseListButtonDestination.y = 20.0f + 3 * (this->_exerciseListButton._sprite.getGlobalBounds().height + 40.0f);
	this->_exerciseListButton._sprite.setPosition(
		0 - this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_exerciseListButton._sprite.getGlobalBounds().width / 2 - 10.0f,
		20.0f + 3 * (this->_exerciseListButton._sprite.getGlobalBounds().height + 40.0f)
	);
}

void ExerciseListState::GUI_HandleInput()
{
	if (this->_data->input.isSpriteClickedAndReleased(this->_menuButton._sprite, sf::Mouse::Left, this->_data->window, this->_menuButton._pressed, this->_menuButton._update))
	{
		if (this->_guiBarDestination.x == 0.0f)
		{
			this->_guiBarDestination.x = 0 - this->_guiBar.getGlobalBounds().width * 0.9;

			this->_menuButtonDestination.x = this->_menuButton._sprite.getGlobalBounds().width;
			this->_homeButtonDestination.x = this->_homeButton._sprite.getGlobalBounds().width;
			this->_homeButtonDestination.y = 20.0f + (this->_homeButton._sprite.getGlobalBounds().height + 40.0f);

			this->_trainingButtonDestination.x = 0 - this->_guiBar.getGlobalBounds().width / 3 - this->_trainingButton._sprite.getGlobalBounds().width / 2 + 10.0f;
			this->_planButtonDestination.x = 0 - this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_planButton._sprite.getGlobalBounds().width / 2 - 10.0f;

			this->_stopwatchButtonDestination.x = 0 - this->_guiBar.getGlobalBounds().width / 3 - this->_stopwatchButton._sprite.getGlobalBounds().width / 2 + 10.0f;
			this->_calculatorButtonDestination.x = 0 - this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_calculatorButton._sprite.getGlobalBounds().width / 2 - 10.0f;

			this->_progressButtonDestination.x = 0 - this->_guiBar.getGlobalBounds().width / 3 - this->_progressButton._sprite.getGlobalBounds().width / 2 + 10.0f;
			this->_exerciseListButtonDestination.x = 0 - this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_exerciseListButton._sprite.getGlobalBounds().width / 2 - 10.0f;
		}
		else
		{
			this->_guiBarDestination.x = 0.0f;

			this->_menuButtonDestination.x = this->_guiBar.getGlobalBounds().width / 3 - this->_menuButton._sprite.getGlobalBounds().width / 2 - 10.0f;
			this->_homeButtonDestination.x = this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_homeButton._sprite.getGlobalBounds().width / 2 + 10.0f;
			this->_homeButtonDestination.y = 20.0f;

			this->_trainingButtonDestination.x = this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_trainingButton._sprite.getGlobalBounds().width / 2 + 10.0f;
			this->_planButtonDestination.x = this->_guiBar.getGlobalBounds().width / 3 - this->_planButton._sprite.getGlobalBounds().width / 2 - 10.0f;

			this->_stopwatchButtonDestination.x = this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_stopwatchButton._sprite.getGlobalBounds().width / 2 + 10.0f;
			this->_calculatorButtonDestination.x = this->_guiBar.getGlobalBounds().width / 3 - this->_calculatorButton._sprite.getGlobalBounds().width / 2 - 10.0f;

			this->_progressButtonDestination.x = this->_guiBar.getGlobalBounds().width * 2 / 3 - this->_progressButton._sprite.getGlobalBounds().width / 2 + 10.0f;
			this->_exerciseListButtonDestination.x = this->_guiBar.getGlobalBounds().width / 3 - this->_exerciseListButton._sprite.getGlobalBounds().width / 2 - 10.0f;
		}
	}

	if (this->_data->input.isSpriteClickedAndReleased(this->_homeButton._sprite, sf::Mouse::Left, this->_data->window, this->_homeButton._pressed, this->_homeButton._update))
	{
		this->_data->machine.AddState(StateRef(new HomeState(this->_data)), true);
	}
	if (this->_data->input.isSpriteClickedAndReleased(this->_trainingButton._sprite, sf::Mouse::Left, this->_data->window, this->_trainingButton._pressed, this->_trainingButton._update))
	{

	}
	if (this->_data->input.isSpriteClickedAndReleased(this->_planButton._sprite, sf::Mouse::Left, this->_data->window, this->_planButton._pressed, this->_planButton._update))
	{

	}
	if (this->_data->input.isSpriteClickedAndReleased(this->_stopwatchButton._sprite, sf::Mouse::Left, this->_data->window, this->_stopwatchButton._pressed, this->_stopwatchButton._update))
	{

	}
	if (this->_data->input.isSpriteClickedAndReleased(this->_calculatorButton._sprite, sf::Mouse::Left, this->_data->window, this->_calculatorButton._pressed, this->_calculatorButton._update))
	{

	}
	if (this->_data->input.isSpriteClickedAndReleased(this->_progressButton._sprite, sf::Mouse::Left, this->_data->window, this->_progressButton._pressed, this->_progressButton._update))
	{

	}
	if (this->_data->input.isSpriteClickedAndReleased(this->_exerciseListButton._sprite, sf::Mouse::Left, this->_data->window, this->_exerciseListButton._pressed, this->_exerciseListButton._update))
	{
		this->_data->machine.AddState(StateRef(new ExerciseListState(this->_data)), true);
	}
}

void ExerciseListState::GUI_Update(float dt)
{
	//	updating button textures
	if (this->_menuButton._update)
	{
		this->_menuButton._update = false;

		if (this->_menuButton._pressed)
		{
			this->_menuButton._rect.left = 300;
			this->_menuButton._sprite.setTextureRect(this->_menuButton._rect);
		}
		else
		{
			this->_menuButton._rect.left = 0;
			this->_menuButton._sprite.setTextureRect(this->_menuButton._rect);
		}
	}
	if (this->_homeButton._update)
	{
		this->_homeButton._update = false;
		if (this->_homeButton._pressed)
		{
			this->_homeButton._rect.left = 300;
			this->_homeButton._sprite.setTextureRect(this->_homeButton._rect);
		}
		else
		{
			this->_homeButton._rect.left = 0;
			this->_homeButton._sprite.setTextureRect(this->_homeButton._rect);
		}
	}
	if (this->_trainingButton._update)
	{
		this->_trainingButton._update = false;
		if (this->_trainingButton._pressed)
		{
			this->_trainingButton._rect.left = 300;
			this->_trainingButton._sprite.setTextureRect(this->_trainingButton._rect);
		}
		else
		{
			this->_trainingButton._rect.left = 0;
			this->_trainingButton._sprite.setTextureRect(this->_trainingButton._rect);
		}
	}
	if (this->_planButton._update)
	{
		this->_planButton._update = false;
		if (this->_planButton._pressed)
		{
			this->_planButton._rect.left = 300;
			this->_planButton._sprite.setTextureRect(this->_planButton._rect);
		}
		else
		{
			this->_planButton._rect.left = 0;
			this->_planButton._sprite.setTextureRect(this->_planButton._rect);
		}
	}
	if (this->_stopwatchButton._update)
	{
		this->_stopwatchButton._update = false;
		if (this->_stopwatchButton._pressed)
		{
			this->_stopwatchButton._rect.left = 300;
			this->_stopwatchButton._sprite.setTextureRect(this->_stopwatchButton._rect);
		}
		else
		{
			this->_stopwatchButton._rect.left = 0;
			this->_stopwatchButton._sprite.setTextureRect(this->_stopwatchButton._rect);
		}
	}
	if (this->_calculatorButton._update)
	{
		this->_calculatorButton._update = false;
		if (this->_calculatorButton._pressed)
		{
			this->_calculatorButton._rect.left = 300;
			this->_calculatorButton._sprite.setTextureRect(this->_calculatorButton._rect);
		}
		else
		{
			this->_calculatorButton._rect.left = 0;
			this->_calculatorButton._sprite.setTextureRect(this->_calculatorButton._rect);
		}
	}
	if (this->_progressButton._update)
	{
		this->_progressButton._update = false;
		if (this->_progressButton._pressed)
		{
			this->_progressButton._rect.left = 300;
			this->_progressButton._sprite.setTextureRect(this->_progressButton._rect);
		}
		else
		{
			this->_progressButton._rect.left = 0;
			this->_progressButton._sprite.setTextureRect(this->_progressButton._rect);
		}
	}
	if (this->_exerciseListButton._update)
	{
		this->_exerciseListButton._update = false;
		if (this->_exerciseListButton._pressed)
		{
			this->_exerciseListButton._rect.left = 300;
			this->_exerciseListButton._sprite.setTextureRect(this->_exerciseListButton._rect);
		}
		else
		{
			this->_exerciseListButton._rect.left = 0;
			this->_exerciseListButton._sprite.setTextureRect(this->_exerciseListButton._rect);
		}
	}

	//	moving objects
	if (floorV2f(this->_guiBar.getPosition()) != floorV2f(this->_guiBarDestination))
	{
		ObjMoveTo(this->_guiBar, this->_guiBarDestination, 650.0f, dt);
	}
	if (floorV2f(this->_menuButton._sprite.getPosition()) != floorV2f(this->_menuButtonDestination))
	{
		ObjMoveTo(this->_menuButton._sprite, this->_menuButtonDestination, 60.0f, dt);
	}
	if (floorV2f(this->_homeButton._sprite.getPosition()) != floorV2f(this->_homeButtonDestination))
	{
		ObjMoveTo(this->_homeButton._sprite, this->_homeButtonDestination, 300.0f, dt);
	}
	if (floorV2f(this->_trainingButton._sprite.getPosition()) != floorV2f(this->_trainingButtonDestination))
	{
		ObjMoveTo(this->_trainingButton._sprite, this->_trainingButtonDestination, 650.0f, dt);
	}
	if (floorV2f(this->_planButton._sprite.getPosition()) != floorV2f(this->_planButtonDestination))
	{
		ObjMoveTo(this->_planButton._sprite, this->_planButtonDestination, 650.0f, dt);
	}
	if (floorV2f(this->_stopwatchButton._sprite.getPosition()) != floorV2f(this->_stopwatchButtonDestination))
	{
		ObjMoveTo(this->_stopwatchButton._sprite, this->_stopwatchButtonDestination, 650.0f, dt);
	}
	if (floorV2f(this->_calculatorButton._sprite.getPosition()) != floorV2f(this->_calculatorButtonDestination))
	{
		ObjMoveTo(this->_calculatorButton._sprite, this->_calculatorButtonDestination, 650.0f, dt);
	}
	if (floorV2f(this->_progressButton._sprite.getPosition()) != floorV2f(this->_progressButtonDestination))
	{
		ObjMoveTo(this->_progressButton._sprite, this->_progressButtonDestination, 650.0f, dt);
	}
	if (floorV2f(this->_exerciseListButton._sprite.getPosition()) != floorV2f(this->_exerciseListButtonDestination))
	{
		ObjMoveTo(this->_exerciseListButton._sprite, this->_exerciseListButtonDestination, 650.0f, dt);
	}
}

void ExerciseListState::GUI_Draw(float dt)
{
	this->_data->window.draw(this->_guiBar);
	this->_data->window.draw(this->_menuButton._sprite);
	this->_data->window.draw(this->_homeButton._sprite);
	this->_data->window.draw(this->_trainingButton._sprite);
	this->_data->window.draw(this->_planButton._sprite);
	this->_data->window.draw(this->_stopwatchButton._sprite);
	this->_data->window.draw(this->_calculatorButton._sprite);
	this->_data->window.draw(this->_progressButton._sprite);
	this->_data->window.draw(this->_exerciseListButton._sprite);
}

void ExerciseListState::Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Deafult Background"));

	GUI_Init();
}

void ExerciseListState::HandleInput()
{
	sf::Event event;

	while (this->_data->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			this->_data->window.close();
		}

		GUI_HandleInput();
	}
}

void ExerciseListState::Update(float dt)
{
	GUI_Update(dt);
}

void ExerciseListState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::Black);

	this->_data->window.draw(this->_background);

	GUI_Draw(dt);

	this->_data->window.display();
}