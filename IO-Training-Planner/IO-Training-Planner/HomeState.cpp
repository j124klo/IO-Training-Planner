#include "DEFINITIONS.hpp"
#include "HomeState.hpp"
//	#include pozosta³e stany do których mo¿na przejœæ z tego stanu

using namespace std;

HomeState::HomeState(AppDataRef data) : _data(data)
{

}

void HomeState::Init()
{
	//this->_data->assets.loadTexture("Home State Background", HOME_STATE_BACKGROUND_FILEPATH);
	this->_data->assets.loadTexture("Deafult Background", DEAFULT_BACKGROUND_FILEPATH);
	this->_data->assets.loadTexture("Gui Bar", GUI_BAR_FILEPATH);
	this->_data->assets.loadTexture("Plan Panel", PLAN_PANEL_FILEPATH);
	this->_data->assets.loadTexture("Menu Button", MENU_BUTTON_FILEPATH);
	this->_data->assets.loadTexture("Home Button", HOME_BUTTON_FILEPATH);

	//this->_background.setTexture(this->_data->assets.getTexture("Home State Background"));
	this->_background.setTexture(this->_data->assets.getTexture("Deafult Background"));
	this->_guiBar.setTexture(this->_data->assets.getTexture("Gui Bar"));
	this->_planPanel.setTexture(this->_data->assets.getTexture("Plan Panel"));
	this->_menuButton.setTexture(this->_data->assets.getTexture("Menu Button"));
	this->_homeButton.setTexture(this->_data->assets.getTexture("Home Button"));

	this->_guiBarDestination.x = 0 - this->_guiBar.getGlobalBounds().width * 3 / 4;
	this->_guiBarDestination.y = 0.0f;
	this->_guiBar.setPosition(0 - this->_guiBar.getGlobalBounds().width *3 /4, 0);
	this->_menuButton.setScale(1.3f, 1.3f);
	this->_menuButton.setPosition(this->_guiBar.getGlobalBounds().width / 2 - this->_menuButton.getGlobalBounds().width / 2, 20.0f);
	this->_homeButton.setScale(1.3f, 1.3f);
	this->_homeButton.setPosition(this->_guiBar.getGlobalBounds().width /2 - this->_homeButton.getGlobalBounds().width /2, 20.0f + (this->_homeButton.getGlobalBounds().height + 40.0f));
	
	this->_planPanel.setScale(0.5f, 0.5f);
	this->_planPanel.setPosition(
		(SCREAN_WIDTH - this->_guiBar.getGlobalBounds().width) / 2 - this->_planPanel.getGlobalBounds().width / 2 + this->_guiBar.getGlobalBounds().width,
		SCREAN_HEIGHT /2 - this->_planPanel.getGlobalBounds().height /2
	);
}

void HomeState::HandleInput()
{
	sf::Event event;

	while (this->_data->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			this->_data->window.close();
		}

		if (this->_data->input.isSpriteClicked(this->_menuButton, sf::Mouse::Left, this->_data->window))
		{
			if (this->_guiBarDestination.x == 0.0f)
			{
				this->_guiBarDestination.x = 0 - this->_guiBar.getGlobalBounds().width * 3 / 4;
			}
			else
			{
				this->_guiBarDestination.x = 0.0f;
			}
		}

	}
}

void HomeState::Update(float dt)
{
	if (floorV2f(this->_guiBar.getPosition()) != floorV2f(this->_guiBarDestination))
	{
		ObjMoveTo(this->_guiBar, this->_guiBarDestination, 650.0f, dt);
	}
}

void HomeState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::Black);

	this->_data->window.draw(this->_background);
	this->_data->window.draw(this->_planPanel);
	this->_data->window.draw(this->_guiBar);
	this->_data->window.draw(this->_menuButton);
	this->_data->window.draw(this->_homeButton);

	this->_data->window.display();
}