#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "AppPlanner.hpp"
#include "Button.hpp"

class RegisterState : public State
{
private:
    AppDataRef _data;
    sf::Clock _clock;

    sf::Sprite _background;

    // Login field
    sf::Text _login;
    sf::Text _loginLabel;
    sf::RectangleShape _loginBounds;
    std::string _loginText;
    bool _isLoginActive;

    // Password field
    sf::Text _password;
    sf::Text _passwordLabel;
    sf::RectangleShape _passwordBounds;
    std::string _passwordText;
    bool _isPasswordActive;

    // Confirm password field
    sf::Text _confirmPassword;
    sf::Text _confirmPasswordLabel;
    sf::RectangleShape _confirmPasswordBounds;
    std::string _confirmPasswordText;
    bool _isConfirmPasswordActive;

    // Error/Success message
    sf::Text _messageLabel;

    // Buttons
    Button _registerButton;
    Button _backButton;

public:
    RegisterState(AppDataRef data);

    void Init();
    void Login_Init();
    void Password_Init();
    void ConfirmPassword_Init();
    void Buttons_Init();
    void Message_Init();

    void HandleInput();
    void Update(float dt);
    void Draw(float dt);
};