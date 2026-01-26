#include "DEFINITIONS.hpp"
#include "RegisterState.hpp"
#include "LoginState.hpp"

using namespace std;

RegisterState::RegisterState(AppDataRef data) : _data(data)
{

}

void RegisterState::Login_Init()
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
		SCREAN_HEIGHT / 7 + 20.0f
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
void RegisterState::Password_Init()
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
void RegisterState::ConfirmPassword_Init()
{
    this->_confirmPassword.setFont(this->_data->assets.getFont("Default Font"));
    this->_confirmPasswordLabel.setFont(this->_data->assets.getFont("Default Font"));

    this->_isConfirmPasswordActive = false;
    this->_confirmPasswordText = "";
    this->_confirmPasswordLabel.setString("Confirm Password:");
    this->_confirmPasswordLabel.setCharacterSize(50);
    this->_confirmPasswordLabel.setFillColor(sf::Color::White);
    this->_confirmPassword.setString("");
    this->_confirmPassword.setCharacterSize(50);
    this->_confirmPassword.setFillColor(sf::Color::Black);
    this->_confirmPasswordBounds.setFillColor(sf::Color::White);
    this->_confirmPasswordBounds.setSize({ 300.0f, 80.0f });

    this->_confirmPasswordBounds.setPosition(
        SCREAN_WIDTH / 2 - this->_confirmPasswordBounds.getGlobalBounds().width / 2,
        this->_passwordBounds.getPosition().y + this->_passwordBounds.getGlobalBounds().height + this->_confirmPasswordLabel.getGlobalBounds().height + 60.0f
    );
    this->_confirmPassword.setPosition(
        this->_confirmPasswordBounds.getPosition().x + this->_confirmPasswordBounds.getSize().x / 2 - this->_confirmPassword.getGlobalBounds().width / 2,
        this->_confirmPasswordBounds.getPosition().y + this->_confirmPasswordBounds.getSize().y / 2 - this->_confirmPassword.getGlobalBounds().height / 2
    );
    this->_confirmPasswordLabel.setPosition(
        this->_confirmPasswordBounds.getPosition().x + this->_confirmPasswordBounds.getSize().x / 2 - this->_confirmPasswordLabel.getGlobalBounds().width / 2,
        this->_confirmPasswordBounds.getPosition().y - this->_confirmPasswordLabel.getGlobalBounds().height - 20.0f
    );
}
void RegisterState::Message_Init()
{
    this->_messageLabel.setFont(this->_data->assets.getFont("Default Font"));
    this->_messageLabel.setString("");
    this->_messageLabel.setCharacterSize(30);
    this->_messageLabel.setFillColor(sf::Color::Red);
    this->_messageLabel.setPosition(
        SCREAN_WIDTH / 2,
        this->_continueButton._sprite.getPosition().y + this->_continueButton._sprite.getGlobalBounds().height + 40.0f
    );
}
void RegisterState::TextFields_HandleInput(sf::Event& event)
{
    // Input field selection
    if (this->_data->input.isRectangleShapeClicked(this->_loginBounds, sf::Mouse::Left, this->_data->window))
    {
        this->_isLoginActive = true;
        this->_isPasswordActive = false;
        this->_isConfirmPasswordActive = false;
        this->_loginBounds.setOutlineColor(sf::Color::Blue);
        this->_loginBounds.setOutlineThickness(2.0f);
        this->_passwordBounds.setOutlineThickness(0.0f);
        this->_confirmPasswordBounds.setOutlineThickness(0.0f);
    }
    else if (this->_data->input.isRectangleShapeClicked(this->_passwordBounds, sf::Mouse::Left, this->_data->window))
    {
        this->_isLoginActive = false;
        this->_isPasswordActive = true;
        this->_isConfirmPasswordActive = false;
        this->_passwordBounds.setOutlineColor(sf::Color::Blue);
        this->_passwordBounds.setOutlineThickness(2.0f);
        this->_loginBounds.setOutlineThickness(0.0f);
        this->_confirmPasswordBounds.setOutlineThickness(0.0f);
    }
    else if (this->_data->input.isRectangleShapeClicked(this->_confirmPasswordBounds, sf::Mouse::Left, this->_data->window))
    {
        this->_isLoginActive = false;
        this->_isPasswordActive = false;
        this->_isConfirmPasswordActive = true;
        this->_confirmPasswordBounds.setOutlineColor(sf::Color::Blue);
        this->_confirmPasswordBounds.setOutlineThickness(2.0f);
        this->_loginBounds.setOutlineThickness(0.0f);
        this->_passwordBounds.setOutlineThickness(0.0f);
    }
    else if (this->_data->input.isSpriteClicked(this->_background, sf::Mouse::Left, this->_data->window))
    {
        this->_isLoginActive = false;
        this->_isPasswordActive = false;
        this->_isConfirmPasswordActive = false;
        this->_loginBounds.setOutlineThickness(0.0f);
        this->_passwordBounds.setOutlineThickness(0.0f);
        this->_confirmPasswordBounds.setOutlineThickness(0.0f);
    }

    // Text input handling
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
    else if (this->_data->input.typingText(event, this->_confirmPasswordText, this->_confirmPassword, this->_isConfirmPasswordActive))
    {
        this->_confirmPassword.setOrigin(this->_confirmPassword.getGlobalBounds().width / 2, this->_confirmPassword.getGlobalBounds().height / 2);
        this->_confirmPassword.setPosition(
            this->_confirmPasswordBounds.getPosition().x + this->_confirmPasswordBounds.getSize().x / 2,
            this->_confirmPasswordBounds.getPosition().y + this->_confirmPasswordBounds.getSize().y / 2
        );
    }
}
void RegisterState::TextFields_Update(float dt)
{

}
void RegisterState::TextFields_Draw(float dt)
{
    this->_data->window.draw(_loginLabel);
    this->_data->window.draw(_loginBounds);
    this->_data->window.draw(_login);

    this->_data->window.draw(_passwordLabel);
    this->_data->window.draw(_passwordBounds);
    this->_data->window.draw(_password);

    this->_data->window.draw(_confirmPasswordLabel);
    this->_data->window.draw(_confirmPasswordBounds);
    this->_data->window.draw(_confirmPassword);

    this->_data->window.draw(_messageLabel);
}

void RegisterState::Buttons_Init()
{
    this->_continueButton._sprite.setTexture(this->_data->assets.getTexture("Continue Button"));
    this->_backButton._sprite.setTexture(this->_data->assets.getTexture("Back Button"));

    this->_continueButton._pressed = false;
    this->_continueButton._update = false;
    this->_continueButton._rect = sf::IntRect(0, 0, 800, 300);
    this->_continueButton._sprite.setTextureRect(this->_continueButton._rect);
    this->_continueButton._sprite.setScale(0.35f, 0.35f);
    this->_continueButton._sprite.setPosition(
        SCREAN_WIDTH / 2 - this->_continueButton._sprite.getGlobalBounds().width / 2,
		this->_confirmPasswordBounds.getPosition().y + this->_confirmPasswordBounds.getGlobalBounds().height + 40.0f
    );

    this->_backButton._pressed = false;
    this->_backButton._update = false;
    this->_backButton._rect = sf::IntRect(0, 0, 300, 300);
    this->_backButton._sprite.setTextureRect(this->_backButton._rect);
    this->_backButton._sprite.setScale(0.35f, 0.35f);
    this->_backButton._sprite.setPosition(
        this->_continueButton._sprite.getPosition().x - 40.0f - this->_backButton._sprite.getGlobalBounds().width,
        this->_continueButton._sprite.getPosition().y
    );
}
void RegisterState::Buttons_HandleInput(sf::Event& event)
{
    if (this->_data->input.isSpriteClickedAndReleased(this->_backButton._sprite, sf::Mouse::Left, this->_data->window, this->_backButton._pressed, this->_backButton._update))
    {
        this->_data->machine.AddState(StateRef(new LoginState(_data)), true);
    }

    if (this->_data->input.isSpriteClickedAndReleased(this->_continueButton._sprite, sf::Mouse::Left, this->_data->window, this->_continueButton._pressed, this->_continueButton._update))
    {
        if (this->_loginText.empty() || this->_passwordText.empty())
        {
            this->_messageLabel.setString("Login i haslo nie moga byc puste!");
            this->_messageLabel.setFillColor(sf::Color::Red);
        }
        else if (this->_passwordText != this->_confirmPasswordText)
        {
            this->_messageLabel.setString("Hasla sie nie zgadzaja!");
            this->_messageLabel.setFillColor(sf::Color::Red);
        }
        else
        {
            auto existingUser = this->_data->database.getUserByLogin(this->_loginText);
            if (existingUser != nullptr)
            {
                this->_messageLabel.setString("Uzytkownik o takim loginie juz istnieje!");
                this->_messageLabel.setFillColor(sf::Color::Red);
            }
            else
            {
                auto newUser = this->_data->database.addUser(this->_loginText, this->_passwordText);

                if (newUser)
                {
                    auto availablePlans = this->_data->database.getAllPlanHeaders();

                    if (!availablePlans.empty())
                    {
                        int defaultPlanId = availablePlans[0].id;
                        this->_data->database.assignPlanToUser(newUser->id, defaultPlanId);
                        // Opcjonalnie: log w konsoli
                        cout << "[REGISTER] Przypisano plan ID: " << defaultPlanId << " uzytkownikowi: " << this->_loginText << endl;
                    }

                    this->_messageLabel.setString("Rejestracja udana! Mozesz sie zalogowac.");
                    this->_messageLabel.setFillColor(sf::Color::Green);
                }
                else
                {
                    this->_messageLabel.setString("Blad podczas rejestracji!");
                    this->_messageLabel.setFillColor(sf::Color::Red);
                }
            }
        }
        this->_messageLabel.setOrigin(this->_messageLabel.getGlobalBounds().width / 2, 0);
    }
}
void RegisterState::Buttons_Update(float dt)
{
    if (this->_continueButton._update)
    {
        this->_continueButton._update = false;
        this->_continueButton._rect.left = this->_continueButton._pressed ? 800 : 0;
        this->_continueButton._sprite.setTextureRect(this->_continueButton._rect);
    }

    if (this->_backButton._update)
    {
        this->_backButton._update = false;
        this->_backButton._rect.left = this->_backButton._pressed ? 300 : 0;
        this->_backButton._sprite.setTextureRect(this->_backButton._rect);
    }
}
void RegisterState::Buttons_Draw(float dt)
{
    this->_data->window.draw(_continueButton._sprite);
    this->_data->window.draw(_backButton._sprite);
}

void RegisterState::Init()
{
    _data->assets.loadTexture("Back Button", BACK_BUTTON_FILEPATH);

    _background.setTexture(_data->assets.getTexture("Default Background"));

    Login_Init();
    Password_Init();
    ConfirmPassword_Init();

    Buttons_Init();

    Message_Init();
}
void RegisterState::HandleInput()
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
void RegisterState::Update(float dt)
{
	TextFields_Update(dt);
	Buttons_Update(dt);
}
void RegisterState::Draw(float dt)
{
    this->_data->window.clear(sf::Color::Black);

    this->_data->window.draw(this->_background);

	TextFields_Draw(dt);
	Buttons_Draw(dt);

    this->_data->window.display();
}