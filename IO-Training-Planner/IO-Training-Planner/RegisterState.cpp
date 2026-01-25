#include "DEFINITIONS.hpp"
#include "RegisterState.hpp"
#include "LoginState.hpp"

using namespace std;

RegisterState::RegisterState(AppDataRef data) : _data(data)
{

}

void RegisterState::Login_Init()
{
    _login.setFont(_data->assets.getFont("Default Font"));
    _loginLabel.setFont(_data->assets.getFont("Default Font"));

    _isLoginActive = false;
    _loginText = "";
    _loginLabel.setString("Login:");
    _loginLabel.setCharacterSize(50);
    _loginLabel.setFillColor(sf::Color::White);
    _login.setString("");
    _login.setCharacterSize(50);
    _login.setFillColor(sf::Color::Black);
    _loginBounds.setFillColor(sf::Color::White);
    _loginBounds.setSize({ 300.0f, 80.0f });
    _loginBounds.setPosition(
        SCREAN_WIDTH / 2 - _loginBounds.getGlobalBounds().width / 2,
        SCREAN_HEIGHT / 5
    );
    _login.setPosition(
        _loginBounds.getPosition().x + _loginBounds.getSize().x / 2 - _login.getGlobalBounds().width / 2,
        _loginBounds.getPosition().y + _loginBounds.getSize().y / 2 - _login.getGlobalBounds().height / 2
    );
    _loginLabel.setPosition(
        _loginBounds.getPosition().x + _loginBounds.getSize().x / 2 - _loginLabel.getGlobalBounds().width / 2,
        _loginBounds.getPosition().y - _loginLabel.getGlobalBounds().height - 20.0f
    );
}
void RegisterState::Password_Init()
{
    _password.setFont(_data->assets.getFont("Default Font"));
    _passwordLabel.setFont(_data->assets.getFont("Default Font"));

    _isPasswordActive = false;
    _passwordText = "";
    _passwordLabel.setString("Password:");
    _passwordLabel.setCharacterSize(50);
    _passwordLabel.setFillColor(sf::Color::White);
    _password.setString("");
    _password.setCharacterSize(50);
    _password.setFillColor(sf::Color::Black);
    _passwordBounds.setFillColor(sf::Color::White);
    _passwordBounds.setSize({ 300.0f, 80.0f });
    _passwordBounds.setPosition(
        SCREAN_WIDTH / 2 - _passwordBounds.getGlobalBounds().width / 2,
        _loginBounds.getPosition().y + 140.0f
    );
    _password.setPosition(
        _passwordBounds.getPosition().x + _passwordBounds.getSize().x / 2 - _password.getGlobalBounds().width / 2,
        _passwordBounds.getPosition().y + _passwordBounds.getSize().y / 2 - _password.getGlobalBounds().height / 2
    );
    _passwordLabel.setPosition(
        _passwordBounds.getPosition().x + _passwordBounds.getSize().x / 2 - _passwordLabel.getGlobalBounds().width / 2,
        _passwordBounds.getPosition().y - _passwordLabel.getGlobalBounds().height - 20.0f
    );
}
void RegisterState::ConfirmPassword_Init()
{
    _confirmPassword.setFont(_data->assets.getFont("Default Font"));
    _confirmPasswordLabel.setFont(_data->assets.getFont("Default Font"));

    _isConfirmPasswordActive = false;
    _confirmPasswordText = "";
    _confirmPasswordLabel.setString("Confirm Password:");
    _confirmPasswordLabel.setCharacterSize(50);
    _confirmPasswordLabel.setFillColor(sf::Color::White);
    _confirmPassword.setString("");
    _confirmPassword.setCharacterSize(50);
    _confirmPassword.setFillColor(sf::Color::Black);
    _confirmPasswordBounds.setFillColor(sf::Color::White);
    _confirmPasswordBounds.setSize({ 300.0f, 80.0f });
    _confirmPasswordBounds.setPosition(
        SCREAN_WIDTH / 2 - _confirmPasswordBounds.getGlobalBounds().width / 2,
        _passwordBounds.getPosition().y + 140.0f
    );
    _confirmPassword.setPosition(
        _confirmPasswordBounds.getPosition().x + _confirmPasswordBounds.getSize().x / 2 - _confirmPassword.getGlobalBounds().width / 2,
        _confirmPasswordBounds.getPosition().y + _confirmPasswordBounds.getSize().y / 2 - _confirmPassword.getGlobalBounds().height / 2
    );
    _confirmPasswordLabel.setPosition(
        _confirmPasswordBounds.getPosition().x + _confirmPasswordBounds.getSize().x / 2 - _confirmPasswordLabel.getGlobalBounds().width / 2,
        _confirmPasswordBounds.getPosition().y - _confirmPasswordLabel.getGlobalBounds().height - 20.0f
    );
}
void RegisterState::Buttons_Init()
{
    // Register button
    _registerButton._sprite.setTexture(_data->assets.getTexture("Continue Button"));
    _registerButton._pressed = false;
    _registerButton._update = false;
    _registerButton._rect = sf::IntRect(0, 0, 800, 300);
    _registerButton._sprite.setTextureRect(_registerButton._rect);
    _registerButton._sprite.setScale(0.25f, 0.25f);
    _registerButton._sprite.setPosition(
        SCREAN_WIDTH / 2 - _registerButton._sprite.getGlobalBounds().width / 2,
        _confirmPasswordBounds.getPosition().y + 140.0f
    );

    // Back button
    _backButton._sprite.setTexture(_data->assets.getTexture("Placeholder Short Button"));
    _backButton._pressed = false;
    _backButton._update = false;
    _backButton._rect = sf::IntRect(0, 0, 800, 300);
    _backButton._sprite.setTextureRect(_backButton._rect);
    _backButton._sprite.setScale(0.2f, 0.2f);
    _backButton._sprite.setPosition(50.0f, 50.0f);
}
void RegisterState::Message_Init()
{
    _messageLabel.setFont(_data->assets.getFont("Default Font"));
    _messageLabel.setString("");
    _messageLabel.setCharacterSize(30);
    _messageLabel.setFillColor(sf::Color::Red);
    _messageLabel.setPosition(
        SCREAN_WIDTH / 2,
        _registerButton._sprite.getPosition().y + 100.0f
    );
}

void RegisterState::Init()
{
    _data->assets.loadTexture("Default Background", DEFAULT_BACKGROUND_FILEPATH);
    _data->assets.loadFont("Default Font", DEFAULT_FONT_FILEPATH);
    _data->assets.loadTexture("Continue Button", CONTINUE_BUTTON_FILEPATH);
    _data->assets.loadTexture("Placeholder Short Button", PLACEHOLDER_SHORT_BUTTON_FILEPATH);

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

    while (_data->window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            _data->window.close();
        }

        // Back button
        if (_data->input.isSpriteClickedAndReleased(_backButton._sprite, sf::Mouse::Left, _data->window, _backButton._pressed, _backButton._update))
        {
            _data->machine.AddState(StateRef(new LoginState(_data)), true);
        }

        // Register button
        if (_data->input.isSpriteClickedAndReleased(_registerButton._sprite, sf::Mouse::Left, _data->window, _registerButton._pressed, _registerButton._update))
        {
            // Validation
            if (_loginText.empty() || _passwordText.empty())
            {
                _messageLabel.setString("Login i haslo nie moga byc puste!");
                _messageLabel.setFillColor(sf::Color::Red);
            }
            else if (_passwordText != _confirmPasswordText)
            {
                _messageLabel.setString("Hasla sie nie zgadzaja!");
                _messageLabel.setFillColor(sf::Color::Red);
            }
            else
            {
                // Check if user already exists
                auto existingUser = _data->database.getUserByLogin(_loginText);
                if (existingUser != nullptr)
                {
                    _messageLabel.setString("Uzytkownik o takim loginie juz istnieje!");
                    _messageLabel.setFillColor(sf::Color::Red);
                }
                else
                {
                    // Register new user
                    auto newUser = _data->database.addUser(_loginText, _passwordText);

                    if (newUser)
                    {
                        // --- AUTOMATYCZNE PRZYPISANIE PLANU ---
                        // Pobieramy wszystkie dostêpne plany
                        auto availablePlans = _data->database.getAllPlanHeaders();

                        // Jeœli w bazie s¹ jakieœ plany, przypisujemy pierwszy (domyœlny)
                        if (!availablePlans.empty())
                        {
                            int defaultPlanId = availablePlans[0].id;
                            _data->database.assignPlanToUser(newUser->id, defaultPlanId);
                            // Opcjonalnie: log w konsoli
                            cout << "[REGISTER] Przypisano plan ID: " << defaultPlanId << " uzytkownikowi: " << _loginText << endl;
                        }
                        // ---------------------------------------

                        _messageLabel.setString("Rejestracja udana! Mozesz sie zalogowac.");
                        _messageLabel.setFillColor(sf::Color::Green);
                    }
                    else
                    {
                        _messageLabel.setString("Blad podczas rejestracji!");
                        _messageLabel.setFillColor(sf::Color::Red);
                    }
                }
            }
            // Center message
            _messageLabel.setOrigin(_messageLabel.getGlobalBounds().width / 2, 0);
        }

        // Input field selection
        if (_data->input.isRectangleShapeClicked(_loginBounds, sf::Mouse::Left, _data->window))
        {
            _isLoginActive = true;
            _isPasswordActive = false;
            _isConfirmPasswordActive = false;
            _loginBounds.setOutlineColor(sf::Color::Blue);
            _loginBounds.setOutlineThickness(2.0f);
            _passwordBounds.setOutlineThickness(0.0f);
            _confirmPasswordBounds.setOutlineThickness(0.0f);
        }
        else if (_data->input.isRectangleShapeClicked(_passwordBounds, sf::Mouse::Left, _data->window))
        {
            _isLoginActive = false;
            _isPasswordActive = true;
            _isConfirmPasswordActive = false;
            _passwordBounds.setOutlineColor(sf::Color::Blue);
            _passwordBounds.setOutlineThickness(2.0f);
            _loginBounds.setOutlineThickness(0.0f);
            _confirmPasswordBounds.setOutlineThickness(0.0f);
        }
        else if (_data->input.isRectangleShapeClicked(_confirmPasswordBounds, sf::Mouse::Left, _data->window))
        {
            _isLoginActive = false;
            _isPasswordActive = false;
            _isConfirmPasswordActive = true;
            _confirmPasswordBounds.setOutlineColor(sf::Color::Blue);
            _confirmPasswordBounds.setOutlineThickness(2.0f);
            _loginBounds.setOutlineThickness(0.0f);
            _passwordBounds.setOutlineThickness(0.0f);
        }
        else if (_data->input.isSpriteClicked(_background, sf::Mouse::Left, _data->window))
        {
            _isLoginActive = false;
            _isPasswordActive = false;
            _isConfirmPasswordActive = false;
            _loginBounds.setOutlineThickness(0.0f);
            _passwordBounds.setOutlineThickness(0.0f);
            _confirmPasswordBounds.setOutlineThickness(0.0f);
        }

        // Text input handling
        if (_data->input.typingText(event, _loginText, _login, _isLoginActive))
        {
            _login.setOrigin(_login.getGlobalBounds().width / 2, _login.getGlobalBounds().height / 2);
            _login.setPosition(
                _loginBounds.getPosition().x + _loginBounds.getSize().x / 2,
                _loginBounds.getPosition().y + _loginBounds.getSize().y / 2
            );
        }
        else if (_data->input.typingText(event, _passwordText, _password, _isPasswordActive))
        {
            _password.setOrigin(_password.getGlobalBounds().width / 2, _password.getGlobalBounds().height / 2);
            _password.setPosition(
                _passwordBounds.getPosition().x + _passwordBounds.getSize().x / 2,
                _passwordBounds.getPosition().y + _passwordBounds.getSize().y / 2
            );
        }
        else if (_data->input.typingText(event, _confirmPasswordText, _confirmPassword, _isConfirmPasswordActive))
        {
            _confirmPassword.setOrigin(_confirmPassword.getGlobalBounds().width / 2, _confirmPassword.getGlobalBounds().height / 2);
            _confirmPassword.setPosition(
                _confirmPasswordBounds.getPosition().x + _confirmPasswordBounds.getSize().x / 2,
                _confirmPasswordBounds.getPosition().y + _confirmPasswordBounds.getSize().y / 2
            );
        }
    }
}
void RegisterState::Update(float dt)
{
    if (_registerButton._update)
    {
        _registerButton._update = false;
        _registerButton._rect.left = _registerButton._pressed ? 800 : 0;
        _registerButton._sprite.setTextureRect(_registerButton._rect);
    }

    if (_backButton._update)
    {
        _backButton._update = false;
        _backButton._rect.left = _backButton._pressed ? 800 : 0;
        _backButton._sprite.setTextureRect(_backButton._rect);
    }
}
void RegisterState::Draw(float dt)
{
    _data->window.clear(sf::Color::Black);

    _data->window.draw(_background);

    _data->window.draw(_loginLabel);
    _data->window.draw(_loginBounds);
    _data->window.draw(_login);

    _data->window.draw(_passwordLabel);
    _data->window.draw(_passwordBounds);
    _data->window.draw(_password);

    _data->window.draw(_confirmPasswordLabel);
    _data->window.draw(_confirmPasswordBounds);
    _data->window.draw(_confirmPassword);

    _data->window.draw(_registerButton._sprite);
    _data->window.draw(_backButton._sprite);
    _data->window.draw(_messageLabel);

    _data->window.display();
}