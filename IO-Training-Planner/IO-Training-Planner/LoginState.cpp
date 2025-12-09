#include "DEFINITIONS.hpp"
#include "LoginState.hpp"
#include "HomeState.hpp"

using namespace std;

LoginState::LoginState(AppDataRef data) : _data(data)
{

}

void LoginState::Init()
{
	this->_data->assets.loadTexture("Deafult Background", DEAFULT_BACKGROUND_FILEPATH);
	this->_data->assets.loadFont("Deafult Font", DEAFULT_FONT_FILEPATH);
	this->_data->assets.loadTexture("Continue Button", CONTINUE_BUTTON_FILEPATH);

	this->_background.setTexture(this->_data->assets.getTexture("Deafult Background"));
	this->_login.setFont(this->_data->assets.getFont("Deafult Font"));
	this->_loginLabel.setFont(this->_data->assets.getFont("Deafult Font"));
	this->_password.setFont(this->_data->assets.getFont("Deafult Font"));
	this->_passwordLabel.setFont(this->_data->assets.getFont("Deafult Font"));
	this->_continueButton.setTexture(this->_data->assets.getTexture("Continue Button"));

	this->_isLoginActive = false;
	this->_loginText = "";
	this->_loginLabel.setString("Login:");
	this->_loginLabel.setCharacterSize(50);
	this->_loginLabel.setFillColor(sf::Color::White);
	this->_login.setString("");
	this->_login.setCharacterSize(50);
	this->_login.setFillColor(sf::Color::Black);
	this->_loginBounds.setFillColor(sf::Color::White);
	this->_loginBounds.setSize({ 300.0f, 100.0f });
	this->_loginBounds.setPosition(
		SCREAN_WIDTH / 2 - this->_loginBounds.getGlobalBounds().width / 2,
		SCREAN_HEIGHT / 3 - this->_loginBounds.getGlobalBounds().height / 2
	);
	this->_login.setPosition(
		this->_loginBounds.getPosition().x + this->_loginBounds.getSize().x / 2 - this->_login.getGlobalBounds().width / 2,
		this->_loginBounds.getPosition().y + this->_loginBounds.getSize().y / 2 - this->_login.getGlobalBounds().height / 2
	);
	this->_loginLabel.setPosition(
		this->_loginBounds.getPosition().x + this->_loginBounds.getSize().x / 2 - this->_loginLabel.getGlobalBounds().width / 2,
		this->_loginBounds.getPosition().y - this->_loginLabel.getGlobalBounds().height - 20.0f
	);

	this->_isPasswordActive = false;
	this->_passwordText = "";
	this->_passwordLabel.setString("Password:");
	this->_passwordLabel.setCharacterSize(50);
	this->_passwordLabel.setFillColor(sf::Color::White);
	this->_password.setString("");
	this->_password.setCharacterSize(50);
	this->_password.setFillColor(sf::Color::Black);
	this->_passwordBounds.setFillColor(sf::Color::White);
	this->_passwordBounds.setSize({ 300.0f, 100.0f });
	this->_passwordBounds.setPosition(
		SCREAN_WIDTH / 2 - this->_passwordBounds.getGlobalBounds().width / 2,
		this->_loginBounds.getPosition().y + this->_passwordBounds.getGlobalBounds().height * 2
	);
	this->_password.setPosition(
		this->_passwordBounds.getPosition().x + this->_passwordBounds.getSize().x / 2 - this->_password.getGlobalBounds().width / 2,
		this->_passwordBounds.getPosition().y + this->_passwordBounds.getSize().y / 2 - this->_password.getGlobalBounds().height / 2
	);
	this->_passwordLabel.setPosition(
		this->_passwordBounds.getPosition().x + this->_passwordBounds.getSize().x / 2 - this->_passwordLabel.getGlobalBounds().width / 2,
		this->_passwordBounds.getPosition().y - this->_passwordLabel.getGlobalBounds().height - 20.0f
	);

	this->_continueButton.setScale(0.8f, 0.8f);
	this->_continueButton.setPosition(
		SCREAN_WIDTH / 2 - this->_continueButton.getGlobalBounds().width / 2,
		this->_passwordBounds.getPosition().y + this->_continueButton.getGlobalBounds().height * 2
	);
}

void LoginState::HandleInput()
{
	sf::Event event;

	while (this->_data->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			this->_data->window.close();
		}

		if (this->_data->input.isSpriteClicked(this->_continueButton, sf::Mouse::Left, this->_data->window))
		{
			auto user = this->_data->database.getUserByLogin(this->_login.getString());
			if (user && user->password == this->_password.getString()) {
				_data->currentUser = user;
				_data->machine.AddState(StateRef(new HomeState(_data)), true);
			}
		}

		if (this->_data->input.isRectangleShapeClicked(this->_loginBounds, sf::Mouse::Left, this->_data->window))
		{
			this->_isLoginActive = true;
			this->_isPasswordActive = false;
			this->_loginBounds.setOutlineColor(sf::Color::Blue);
			this->_loginBounds.setOutlineThickness(2.0f);
			this->_passwordBounds.setOutlineThickness(0.0f);
		}
		else if (this->_data->input.isRectangleShapeClicked(this->_passwordBounds, sf::Mouse::Left, this->_data->window))
		{
			this->_isLoginActive = false;
			this->_isPasswordActive = true;
			this->_passwordBounds.setOutlineColor(sf::Color::Blue);
			this->_passwordBounds.setOutlineThickness(2.0f);
			this->_loginBounds.setOutlineThickness(0.0f);
		}
		else if (this->_data->input.isSpriteClicked(this->_background, sf::Mouse::Left, this->_data->window))
		{
			this->_isLoginActive = false;
			this->_isPasswordActive = false;
			this->_loginBounds.setOutlineThickness(0.0f);
			this->_passwordBounds.setOutlineThickness(0.0f);
		}

		if (this->_data->input.typingText(event, this->_loginText, this->_login, this->_isLoginActive))
		{
			this->_login.setOrigin(this->_login.getGlobalBounds().width / 2, this->_login.getGlobalBounds().height / 2);
			this->_login.setPosition(
				this->_loginBounds.getPosition().x + this->_loginBounds.getSize().x / 2,
				this->_loginBounds.getPosition().y + this->_loginBounds.getSize().y / 2
			);
		}
		else if (this->_data->input.typingText(event, this->_passwordText, this->_password, this->_isPasswordActive))
		{
			this->_password.setOrigin(this->_password.getGlobalBounds().width / 2, this->_password.getGlobalBounds().height / 2);
			this->_password.setPosition(
				this->_passwordBounds.getPosition().x + this->_passwordBounds.getSize().x / 2,
				this->_passwordBounds.getPosition().y + this->_passwordBounds.getSize().y / 2
			);
		}
	}
}

void LoginState::Update(float dt)
{
	//	tu jak coœ przesuwamy po ekrania, zmieniamy czy jest widoczne itp (np pasek menu najlepiej za³adowaæ poza ekranem i go wysun¹æ na ekran)
}

void LoginState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::Black);

	this->_data->window.draw(this->_background);

	this->_data->window.draw(this->_loginLabel);
	this->_data->window.draw(this->_loginBounds);
	this->_data->window.draw(this->_login);

	this->_data->window.draw(this->_passwordLabel);
	this->_data->window.draw(this->_passwordBounds);
	this->_data->window.draw(this->_password);

	this->_data->window.draw(this->_continueButton);

	this->_data->window.display();
}