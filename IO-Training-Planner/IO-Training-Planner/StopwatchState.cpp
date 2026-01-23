#include "DEFINITIONS.hpp"
#include "StopwatchState.hpp"

using namespace std;

StopwatchState::StopwatchState(AppDataRef data) : GuiState(data)
{
    _elapsedTime = 0.0f;
    _isRunning = false;
}

string StopwatchState::formatTime(float seconds)
{
    int hours = static_cast<int>(seconds) / 3600;
    int minutes = (static_cast<int>(seconds) % 3600) / 60;
    int secs = static_cast<int>(seconds) % 60;
    int millisecs = static_cast<int>((seconds - static_cast<int>(seconds)) * 100);

    ostringstream oss;
    oss << setfill('0') << setw(2) << hours << ":"
        << setfill('0') << setw(2) << minutes << ":"
        << setfill('0') << setw(2) << secs << "."
        << setfill('0') << setw(2) << millisecs;

    return oss.str();
}

void StopwatchState::State_Init()
{
    this->_background.setTexture(this->_data->assets.getTexture("Default Background"));

    // Konfiguracja wyœwietlania czasu
    this->_timeDisplay.setFont(this->_data->assets.getFont("Default Font"));
    this->_timeDisplay.setString("00:00:00.00");
    this->_timeDisplay.setCharacterSize(72);
    this->_timeDisplay.setFillColor(sf::Color::White);

    // Centrowanie tekstu z czasem
    sf::FloatRect timeBounds = this->_timeDisplay.getLocalBounds();
    this->_timeDisplay.setOrigin(timeBounds.width / 2.0f, timeBounds.height / 2.0f);
    this->_timeDisplay.setPosition(SCREAN_WIDTH / 2.0f, SCREAN_HEIGHT / 3.0f);

    // Przycisk Start/Stop
    float buttonWidth = 150.0f;
    float buttonHeight = 50.0f;
    float buttonY = SCREAN_HEIGHT / 2.0f + 50.0f;
    float spacing = 30.0f;

    // Start/Stop button setup
    this->_startStopButton._accessible = true;
    this->_startStopButton._pressed = false;
    this->_startStopButton._update = false;

    this->_startStopText.setFont(this->_data->assets.getFont("Default Font"));
    this->_startStopText.setString("START");
    this->_startStopText.setCharacterSize(24);
    this->_startStopText.setFillColor(sf::Color::White);

    sf::FloatRect startBounds = this->_startStopText.getLocalBounds();
    this->_startStopText.setOrigin(startBounds.width / 2.0f, startBounds.height / 2.0f);
    this->_startStopText.setPosition(SCREAN_WIDTH / 2.0f - buttonWidth / 2.0f - spacing, buttonY);

    // Reset button setup
    this->_resetButton._accessible = true;
    this->_resetButton._pressed = false;
    this->_resetButton._update = false;

    this->_resetText.setFont(this->_data->assets.getFont("Default Font"));
    this->_resetText.setString("RESET");
    this->_resetText.setCharacterSize(24);
    this->_resetText.setFillColor(sf::Color::White);

    sf::FloatRect resetBounds = this->_resetText.getLocalBounds();
    this->_resetText.setOrigin(resetBounds.width / 2.0f, resetBounds.height / 2.0f);
    this->_resetText.setPosition(SCREAN_WIDTH / 2.0f + buttonWidth / 2.0f + spacing, buttonY);

    // Restart zegara
    this->_clock.restart();
}

void StopwatchState::State_HandleInput(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(this->_data->window);

        // Sprawdzenie klikniêcia Start/Stop (prostok¹tny obszar wokó³ tekstu)
        sf::FloatRect startStopArea = this->_startStopText.getGlobalBounds();
        startStopArea.left -= 20.0f;
        startStopArea.top -= 10.0f;
        startStopArea.width += 40.0f;
        startStopArea.height += 20.0f;

        if (startStopArea.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
        {
            this->_isRunning = !this->_isRunning;

            if (this->_isRunning)
            {
                this->_startStopText.setString("STOP");
                this->_startStopText.setFillColor(sf::Color::Red);
                this->_clock.restart(); // Restart zegara przy wznowieniu
            }
            else
            {
                this->_startStopText.setString("START");
                this->_startStopText.setFillColor(sf::Color::Green);
            }

            // Ponowne wycentrowanie tekstu
            sf::FloatRect bounds = this->_startStopText.getLocalBounds();
            this->_startStopText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        }

        // Sprawdzenie klikniêcia Reset
        sf::FloatRect resetArea = this->_resetText.getGlobalBounds();
        resetArea.left -= 20.0f;
        resetArea.top -= 10.0f;
        resetArea.width += 40.0f;
        resetArea.height += 20.0f;

        if (resetArea.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
        {
            this->_elapsedTime = 0.0f;
            this->_isRunning = false;
            this->_startStopText.setString("START");
            this->_startStopText.setFillColor(sf::Color::Green);

            // Ponowne wycentrowanie tekstu
            sf::FloatRect bounds = this->_startStopText.getLocalBounds();
            this->_startStopText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

            this->_timeDisplay.setString(formatTime(0.0f));
            this->_clock.restart();
        }
    }
}

void StopwatchState::State_Update(float dt)
{
    if (this->_isRunning)
    {
        this->_elapsedTime += this->_clock.restart().asSeconds();
        this->_timeDisplay.setString(formatTime(this->_elapsedTime));

        // Ponowne wycentrowanie po aktualizacji tekstu
        sf::FloatRect timeBounds = this->_timeDisplay.getLocalBounds();
        this->_timeDisplay.setOrigin(timeBounds.width / 2.0f, timeBounds.height / 2.0f);
    }
    else
    {
        this->_clock.restart(); // Zapobiega "skokowi" czasu po wznowieniu
    }
}

void StopwatchState::State_Draw(float dt)
{
    this->_data->window.draw(this->_background);
    this->_data->window.draw(this->_timeDisplay);
    this->_data->window.draw(this->_startStopText);
    this->_data->window.draw(this->_resetText);
}