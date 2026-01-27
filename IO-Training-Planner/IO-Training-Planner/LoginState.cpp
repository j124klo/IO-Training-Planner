#include "DEFINITIONS.hpp"
#include "LoginState.hpp"
#include "HomeState.hpp"
#include "RegisterState.hpp"

using namespace std;

LoginState::LoginState(AppDataRef data) : _data(data)
{

}

void LoginState::Login_Init()
{
	this->_login.setFont(this->_data->assets.getFont("Default Font"));
	this->_loginLabel.setFont(this->_data->assets.getFont("Default Font"));

	this->_isLoginActive = false;
	this->_loginText = "";
	this->_loginLabel.setString("Login:");
	this->_loginLabel.setCharacterSize(50);
	this->_loginLabel.setFillColor(sf::Color::White);
	this->_login.setString("");
	this->_login.setCharacterSize(50);
	this->_login.setFillColor(sf::Color::Black);
	this->_loginBounds.setFillColor(sf::Color::White);
	this->_loginBounds.setSize({ 300.0f, 80.0f });

	this->_loginBounds.setPosition(
		SCREAN_WIDTH / 2 - this->_loginBounds.getGlobalBounds().width / 2,
		SCREAN_HEIGHT / 7 +20.0f
	);
	this->_login.setPosition(
		this->_loginBounds.getPosition().x + this->_loginBounds.getSize().x / 2 - this->_login.getGlobalBounds().width / 2,
		this->_loginBounds.getPosition().y + this->_loginBounds.getSize().y / 2 - this->_login.getGlobalBounds().height / 2
	);
	this->_loginLabel.setPosition(
		this->_loginBounds.getPosition().x + this->_loginBounds.getSize().x / 2 - this->_loginLabel.getGlobalBounds().width / 2,
		this->_loginBounds.getPosition().y - this->_loginLabel.getGlobalBounds().height - 20.0f
	);
}
void LoginState::Password_Init()
{
	this->_password.setFont(this->_data->assets.getFont("Default Font"));
	this->_passwordLabel.setFont(this->_data->assets.getFont("Default Font"));

	this->_isPasswordActive = false;
	this->_passwordText = "";
	this->_passwordLabel.setString("Password:");
	this->_passwordLabel.setCharacterSize(50);
	this->_passwordLabel.setFillColor(sf::Color::White);
	this->_password.setString("");
	this->_password.setCharacterSize(50);
	this->_password.setFillColor(sf::Color::Black);
	this->_passwordBounds.setFillColor(sf::Color::White);
	this->_passwordBounds.setSize({ 300.0f, 80.0f });

	this->_passwordBounds.setPosition(
		SCREAN_WIDTH / 2 - this->_passwordBounds.getGlobalBounds().width / 2,
		this->_loginBounds.getPosition().y + this->_loginBounds.getGlobalBounds().height + this->_passwordLabel.getGlobalBounds().height + 60.0f
	);
	this->_password.setPosition(
		this->_passwordBounds.getPosition().x + this->_passwordBounds.getSize().x / 2 - this->_password.getGlobalBounds().width / 2,
		this->_passwordBounds.getPosition().y + this->_passwordBounds.getSize().y / 2 - this->_password.getGlobalBounds().height / 2
	);
	this->_passwordLabel.setPosition(
		this->_passwordBounds.getPosition().x + this->_passwordBounds.getSize().x / 2 - this->_passwordLabel.getGlobalBounds().width / 2,
		this->_passwordBounds.getPosition().y - this->_passwordLabel.getGlobalBounds().height - 20.0f
	);
}

// --- NOWA FUNKCJA INICJALIZUJ¥CA TEKST B£ÊDU ---
void LoginState::ErrorMessage_Init()
{
	this->_errorMessage.setFont(this->_data->assets.getFont("Default Font"));
	this->_errorMessage.setString(""); // Na pocz¹tku puste
	this->_errorMessage.setCharacterSize(30);
	this->_errorMessage.setFillColor(sf::Color::Red); // Czerwony kolor b³êdu
	// Pozycja nad przyciskiem Continue (lub pod has³em)
	this->_errorMessage.setPosition(SCREAN_WIDTH / 2, 0);
}

// --- NOWA FUNKCJA DO MASKOWANIA HAS£A ---
void LoginState::UpdatePasswordMask()
{
	// Tworzymy ci¹g gwiazdek o d³ugoœci prawdziwego has³a
	string masked = "";
	for (size_t i = 0; i < _passwordText.length(); i++) {
		masked += "*";
	}

	// Ustawiamy gwiazdki w obiekcie graficznym
	this->_password.setString(masked);

	// Ponowne centrowanie tekstu (bo gwiazdki maj¹ inn¹ szerokoœæ ni¿ litery)
	this->_password.setOrigin(this->_password.getGlobalBounds().width / 2, this->_password.getGlobalBounds().height / 2);
	this->_password.setPosition(
		this->_passwordBounds.getPosition().x + this->_passwordBounds.getSize().x / 2,
		this->_passwordBounds.getPosition().y - 15 + this->_passwordBounds.getSize().y / 2
	);
}

void LoginState::TextFields_HandleInput(sf::Event& event)
{
	// Obs³uga klikniêæ myszk¹ (bez zmian)
	if (this->_data->input.isRectangleShapeClicked(this->_loginBounds, sf::Mouse::Left, this->_data->window))
	{
		this->_isLoginActive = true;
		this->_isPasswordActive = false;
		this->_loginBounds.setOutlineColor(sf::Color::Blue);
		this->_loginBounds.setOutlineThickness(2.0f);
		this->_passwordBounds.setOutlineThickness(0.0f);
		this->_errorMessage.setString("");
	}
	else if (this->_data->input.isRectangleShapeClicked(this->_passwordBounds, sf::Mouse::Left, this->_data->window))
	{
		this->_isLoginActive = false;
		this->_isPasswordActive = true;
		this->_passwordBounds.setOutlineColor(sf::Color::Blue);
		this->_passwordBounds.setOutlineThickness(2.0f);
		this->_loginBounds.setOutlineThickness(0.0f);
		this->_errorMessage.setString("");
	}
	else if (this->_data->input.isSpriteClicked(this->_background, sf::Mouse::Left, this->_data->window))
	{
		this->_isLoginActive = false;
		this->_isPasswordActive = false;
		this->_loginBounds.setOutlineThickness(0.0f);
		this->_passwordBounds.setOutlineThickness(0.0f);
	}

	// --- SEKCJA PISANIA Z OGRANICZENIEM DO 10 ZNAKÓW ---

	// 1. LOGIN
	if (this->_data->input.typingText(event, this->_loginText, this->_login, this->_isLoginActive))
	{
		// OGRANICZENIE: Jeœli d³u¿sze ni¿ 10, usuñ ostatni znak
		if (this->_loginText.length() > 10) {
			this->_loginText.pop_back();
			this->_login.setString(this->_loginText); // Aktualizacja wygl¹du
		}

		// Pozycjonowanie (centrowanie)
		this->_login.setOrigin(this->_login.getGlobalBounds().width / 2, this->_login.getGlobalBounds().height / 2);
		this->_login.setPosition(
			this->_loginBounds.getPosition().x + this->_loginBounds.getSize().x / 2,
			this->_loginBounds.getPosition().y - 15 + this->_loginBounds.getSize().y / 2
		);

		if (!this->_errorMessage.getString().isEmpty()) this->_errorMessage.setString("");
	}
	// 2. HAS£O
	else if (this->_data->input.typingText(event, this->_passwordText, this->_password, this->_isPasswordActive))
	{
		// OGRANICZENIE: Jeœli d³u¿sze ni¿ 10, usuñ ostatni znak
		if (this->_passwordText.length() > 10) {
			this->_passwordText.pop_back();
			// UpdatePasswordMask() zaraz zaktualizuje wygl¹d (gwiazdki), wiêc nie musimy tu robiæ setString
		}

		UpdatePasswordMask(); // Zamiana na gwiazdki

		if (!this->_errorMessage.getString().isEmpty()) this->_errorMessage.setString("");
	}
}

void LoginState::TextFields_Update(float dt)
{

}
void LoginState::TextFields_Draw(float dt)
{
	this->_data->window.draw(this->_loginLabel);
	this->_data->window.draw(this->_loginBounds);
	this->_data->window.draw(this->_login);

	this->_data->window.draw(this->_passwordLabel);
	this->_data->window.draw(this->_passwordBounds);
	this->_data->window.draw(this->_password);
	this->_data->window.draw(this->_errorMessage);
}

void LoginState::Buttons_Init()
{
	this->_data->assets.loadTexture("Continue Button", CONTINUE_BUTTON_FILEPATH);
	this->_data->assets.loadTexture("Register Button", REGISTER_BUTTON_FILEPATH);

	this->_continueButton._sprite.setTexture(this->_data->assets.getTexture("Continue Button"));
	this->_registerButton._sprite.setTexture(this->_data->assets.getTexture("Register Button"));

	this->_continueButton._pressed = false;
	this->_continueButton._update = false;
	this->_continueButton._rect = sf::IntRect(0, 0, 800, 300);
	this->_continueButton._sprite.setTextureRect(this->_continueButton._rect);
	this->_continueButton._sprite.setScale(0.35f, 0.35f);
	this->_continueButton._sprite.setPosition(
		SCREAN_WIDTH / 2 - this->_continueButton._sprite.getGlobalBounds().width / 2,
		this->_passwordBounds.getPosition().y + this->_passwordBounds.getGlobalBounds().height + 40.0f
	);

	this->_registerButton._pressed = false;
	this->_registerButton._update = false;
	this->_registerButton._rect = sf::IntRect(0, 0, 800, 300);
	this->_registerButton._sprite.setTextureRect(this->_registerButton._rect);
	this->_registerButton._sprite.setScale(0.35f, 0.35f);
	this->_registerButton._sprite.setPosition(
		SCREAN_WIDTH / 2 - this->_registerButton._sprite.getGlobalBounds().width / 2,
		this->_continueButton._sprite.getPosition().y + this->_continueButton._sprite.getGlobalBounds().height + 40.0f
	);
}
void LoginState::Buttons_HandleInput(sf::Event& event)
{
	if (this->_data->input.isSpriteClickedAndReleased(this->_continueButton._sprite, sf::Mouse::Left, this->_data->window, this->_continueButton._pressed, this->_continueButton._update))
	{
		auto user = this->_data->database.getUserByLogin(this->_loginText); // U¿ywamy _loginText (string), a nie _login.getString()

		// Sprawdzamy has³o u¿ywaj¹c _passwordText (prawdziwe dane), a nie _password (gwiazdki)
		if (user && user->password == this->_passwordText) {
			this->_data->currentUser = user;
			this->_data->machine.AddState(StateRef(new HomeState(this->_data)), true);
		}
		else {
			// --- B£¥D LOGOWANIA ---
			this->_errorMessage.setString("Invalid login or password!");

			// Centrowanie komunikatu b³êdu
			sf::FloatRect textRect = this->_errorMessage.getLocalBounds();
			this->_errorMessage.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

			// Pozycja: Pomiêdzy polem has³a a przyciskiem Continue
			this->_errorMessage.setPosition(
				SCREAN_WIDTH / 2.0f,
				this->_passwordBounds.getPosition().y + this->_passwordBounds.getSize().y + 20.0f
			);
		}
	}

	if (this->_data->input.isSpriteClickedAndReleased(this->_registerButton._sprite, sf::Mouse::Left, this->_data->window, this->_registerButton._pressed, this->_registerButton._update))
	{
		this->_data->machine.AddState(StateRef(new RegisterState(this->_data)), true);
	}
}
void LoginState::Buttons_Update(float dt)
{
	if (this->_continueButton._update)
	{
		this->_continueButton._update = false;

		if (this->_continueButton._pressed)
		{
			this->_continueButton._rect.left = 800;
			this->_continueButton._sprite.setTextureRect(this->_continueButton._rect);
		}
		else
		{
			this->_continueButton._rect.left = 0;
			this->_continueButton._sprite.setTextureRect(this->_continueButton._rect);
		}
	}

	if (this->_registerButton._update)
	{
		this->_registerButton._update = false;

		if (this->_registerButton._pressed)
		{
			this->_registerButton._rect.left = 800;
			this->_registerButton._sprite.setTextureRect(this->_registerButton._rect);
		}
		else
		{
			this->_registerButton._rect.left = 0;
			this->_registerButton._sprite.setTextureRect(this->_registerButton._rect);
		}
	}
}
void LoginState::Buttons_Draw(float dt)
{
	this->_data->window.draw(this->_continueButton._sprite);
	this->_data->window.draw(this->_registerButton._sprite);
}

void LoginState::Init()
{
	this->_data->assets.loadTexture("Default Background", DEFAULT_BACKGROUND_FILEPATH);
	this->_data->assets.loadFont("Default Font", DEFAULT_FONT_FILEPATH);

	this->_background.setTexture(this->_data->assets.getTexture("Default Background"));
	
	Login_Init();
	Password_Init();
	ErrorMessage_Init();
	Buttons_Init();
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

		Buttons_HandleInput(event);
		TextFields_HandleInput(event);
	}
}
void LoginState::Update(float dt)
{
	Buttons_Update(dt);
	TextFields_Update(dt);
}
void LoginState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::Black);

	this->_data->window.draw(this->_background);

	TextFields_Draw(dt);
	Buttons_Draw(dt);

	this->_data->window.display();
}