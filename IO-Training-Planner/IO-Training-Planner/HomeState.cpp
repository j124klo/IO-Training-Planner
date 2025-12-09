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
	//	reszta co ma siê za³adowaæ tutaj

	//this->_background.setTexture(this->_data->assets.getTexture("Home State Background"));
	this->_background.setTexture(this->_data->assets.getTexture("Deafult Background"));
	//	reszta co ma siê ustawiæ tutaj

	
	//	tak siê ustawia elementy w oknie  |
	//									  V

	/*this->_BlackJack_station.setPosition(
		((SCREAN_WIDTH / 2) - (this->_BlackJack_station.getGlobalBounds().width / 2)),
		((SCREAN_HEIGHT / 2) - (this->_BlackJack_station.getGlobalBounds().height / 2))
	);
	this->_gui_bar.setPosition(0, (SCREAN_HEIGHT - this->_gui_bar.getGlobalBounds().height));*/
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

		//	tu dodaje sie akcje typu klikanie elementów i przycisków (wszystko mo¿e byæ przyciskiem, nie ma ograniczeñ, nawet background)

	}
}

void HomeState::Update(float dt)
{
	//	tu jak coœ przesuwamy po ekrania, zmieniamy czy jest widoczne itp (np pasek menu najlepiej za³adowaæ poza ekranem i go wysun¹æ na ekran)
}

void HomeState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::Black);

	this->_data->window.draw(this->_background);
	//	tu co nad czym ma siê wyœwietlaæ (im wy¿ej w kodzie tym bardziej na spodzie)

	this->_data->window.display();
}