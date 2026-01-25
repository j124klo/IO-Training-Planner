#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "AppPlanner.hpp"
#include "Button.hpp"

using namespace std;

class LoginState : public State
{
private:
	AppDataRef _data;
	sf::Clock _clock;

	sf::Sprite _background;

	sf::Text _login;
	sf::Text _loginLabel;
	sf::RectangleShape _loginBounds;
	string _loginText;
	bool _isLoginActive;

	sf::Text _password;
	sf::Text _passwordLabel;
	sf::RectangleShape _passwordBounds;
	string _passwordText;
	bool _isPasswordActive;

	Button _continueButton;
	Button _registerButton;

public:
	LoginState(AppDataRef data);

	void Login_Init();
	void Password_Init();
	void TextFields_HandleInput(sf::Event& event);
	void TextFields_Update(float dt);
	void TextFields_Draw(float dt);

	void Buttons_Init();
	void Buttons_HandleInput(sf::Event& event);
	void Buttons_Update(float dt);
	void Buttons_Draw(float dt);

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
};