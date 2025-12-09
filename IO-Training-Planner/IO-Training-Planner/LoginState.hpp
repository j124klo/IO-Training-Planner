#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "AppPlanner.hpp"

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

	sf::Sprite _continueButton;

public:
	LoginState(AppDataRef data);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
};