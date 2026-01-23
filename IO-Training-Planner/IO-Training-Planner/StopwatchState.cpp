#include "DEFINITIONS.hpp"
#include "StopwatchState.hpp"

using namespace std;

StopwatchState::StopwatchState(AppDataRef data) : GuiState(data)
{
    _elapsedTime = 0.0f;
    _isRunning = false;
    _mode = StopwatchMode::Stopwatch;
    _timerDuration = 0.0f;
    _timerMinutes = 1;
    _timerSeconds = 0;
    _timerFinished = false;
}

string StopwatchState::formatTime(float seconds)
{
    int hours = static_cast<int>(seconds) / 3600;
    int minutes = (static_cast<int>(seconds) % 3600) / 60;
    int secs = static_cast<int>(seconds) % 60;
    int millisecs = static_cast<int>((seconds - static_cast<int>(seconds)) * 100);

    ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours << ":"
        << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << secs << "."
        << std::setfill('0') << std::setw(2) << millisecs;

    return oss.str();
}

void StopwatchState::setupButton(sf::Text& text, const string& label, float x, float y)
{
    text.setFont(this->_data->assets.getFont("Default Font"));
    text.setString(label);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    text.setPosition(x, y);
}

void StopwatchState::updateLapDisplay()
{
    _lapTexts.clear();
    float startY = SCREAN_HEIGHT / 2.0f + 130.0f;
    int displayCount = min(static_cast<int>(_laps.size()), 5);

    for (int i = 0; i < displayCount; i++)
    {
        int lapIndex = static_cast<int>(_laps.size()) - 1 - i;
        sf::Text lapText;
        lapText.setFont(this->_data->assets.getFont("Default Font"));

        ostringstream oss;
        oss << "Lap " << (lapIndex + 1) << ": " << formatTime(_laps[lapIndex]);
        lapText.setString(oss.str());
        lapText.setCharacterSize(18);
        lapText.setFillColor(sf::Color::Yellow);

        sf::FloatRect bounds = lapText.getLocalBounds();
        lapText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        lapText.setPosition(SCREAN_WIDTH / 2.0f, startY + i * 25.0f);

        _lapTexts.push_back(lapText);
    }
}

void StopwatchState::State_Init()
{
    this->_background.setTexture(this->_data->assets.getTexture("Default Background"));

    // Wyœwietlanie czasu
    this->_timeDisplay.setFont(this->_data->assets.getFont("Default Font"));
    this->_timeDisplay.setString("00:00:00.00");
    this->_timeDisplay.setCharacterSize(72);
    this->_timeDisplay.setFillColor(sf::Color::White);

    sf::FloatRect timeBounds = this->_timeDisplay.getLocalBounds();
    this->_timeDisplay.setOrigin(timeBounds.width / 2.0f, timeBounds.height / 2.0f);
    this->_timeDisplay.setPosition(SCREAN_WIDTH / 2.0f, SCREAN_HEIGHT / 3.0f);

    // Label trybu
    this->_modeLabel.setFont(this->_data->assets.getFont("Default Font"));
    this->_modeLabel.setString("STOPER");
    this->_modeLabel.setCharacterSize(28);
    this->_modeLabel.setFillColor(sf::Color::Cyan);
    sf::FloatRect modeBounds = this->_modeLabel.getLocalBounds();
    this->_modeLabel.setOrigin(modeBounds.width / 2.0f, modeBounds.height / 2.0f);
    this->_modeLabel.setPosition(SCREAN_WIDTH / 2.0f, SCREAN_HEIGHT / 3.0f - 80.0f);

    // Pozycje przycisków
    float buttonY = SCREAN_HEIGHT / 2.0f + 50.0f;
    float spacing = 100.0f;

    // Start/Stop
    setupButton(this->_startStopText, "START", SCREAN_WIDTH / 2.0f - spacing * 1.5f, buttonY);
    this->_startStopText.setFillColor(sf::Color::Green);

    // Reset
    setupButton(this->_resetText, "RESET", SCREAN_WIDTH / 2.0f - spacing * 0.5f, buttonY);

    // Lap
    setupButton(this->_lapText, "LAP", SCREAN_WIDTH / 2.0f + spacing * 0.5f, buttonY);
    this->_lapText.setFillColor(sf::Color::Yellow);

    // Mode switch
    setupButton(this->_modeText, "TIMER", SCREAN_WIDTH / 2.0f + spacing * 1.5f, buttonY);
    this->_modeText.setFillColor(sf::Color::Magenta);

    // Timer settings
    float timerY = SCREAN_HEIGHT / 2.0f - 30.0f;

    this->_timerSettingText.setFont(this->_data->assets.getFont("Default Font"));
    this->_timerSettingText.setString("01:00");
    this->_timerSettingText.setCharacterSize(32);
    this->_timerSettingText.setFillColor(sf::Color::White);
    sf::FloatRect tsBounds = this->_timerSettingText.getLocalBounds();
    this->_timerSettingText.setOrigin(tsBounds.width / 2.0f, tsBounds.height / 2.0f);
    this->_timerSettingText.setPosition(SCREAN_WIDTH / 2.0f, timerY);

    setupButton(this->_plusMinText, "+M", SCREAN_WIDTH / 2.0f - 80.0f, timerY - 30.0f);
    setupButton(this->_minusMinText, "-M", SCREAN_WIDTH / 2.0f - 80.0f, timerY + 30.0f);
    setupButton(this->_plusSecText, "+S", SCREAN_WIDTH / 2.0f + 80.0f, timerY - 30.0f);
    setupButton(this->_minusSecText, "-S", SCREAN_WIDTH / 2.0f + 80.0f, timerY + 30.0f);

    this->_clock.restart();
}

void StopwatchState::State_HandleInput(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(this->_data->window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        // Start/Stop
        sf::FloatRect startStopArea = this->_startStopText.getGlobalBounds();
        startStopArea.left -= 20.0f; startStopArea.top -= 10.0f;
        startStopArea.width += 40.0f; startStopArea.height += 20.0f;

        if (startStopArea.contains(mousePosF))
        {
            if (_mode == StopwatchMode::Timer && !_isRunning && _elapsedTime == 0.0f)
            {
                _timerDuration = static_cast<float>(_timerMinutes * 60 + _timerSeconds);
                _elapsedTime = _timerDuration;
            }

            this->_isRunning = !this->_isRunning;
            _timerFinished = false;

            if (this->_isRunning)
            {
                this->_startStopText.setString("STOP");
                this->_startStopText.setFillColor(sf::Color::Red);
                this->_clock.restart();
            }
            else
            {
                this->_startStopText.setString("START");
                this->_startStopText.setFillColor(sf::Color::Green);
            }

            sf::FloatRect bounds = this->_startStopText.getLocalBounds();
            this->_startStopText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        }

        // Reset
        sf::FloatRect resetArea = this->_resetText.getGlobalBounds();
        resetArea.left -= 20.0f; resetArea.top -= 10.0f;
        resetArea.width += 40.0f; resetArea.height += 20.0f;

        if (resetArea.contains(mousePosF))
        {
            this->_elapsedTime = 0.0f;
            this->_isRunning = false;
            this->_timerFinished = false;
            this->_laps.clear();
            this->_lapTexts.clear();
            this->_startStopText.setString("START");
            this->_startStopText.setFillColor(sf::Color::Green);

            sf::FloatRect bounds = this->_startStopText.getLocalBounds();
            this->_startStopText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

            this->_timeDisplay.setString(formatTime(0.0f));
            this->_timeDisplay.setFillColor(sf::Color::White);
            this->_clock.restart();
        }

        // Lap (tylko w trybie Stopwatch)
        if (_mode == StopwatchMode::Stopwatch)
        {
            sf::FloatRect lapArea = this->_lapText.getGlobalBounds();
            lapArea.left -= 20.0f; lapArea.top -= 10.0f;
            lapArea.width += 40.0f; lapArea.height += 20.0f;

            if (lapArea.contains(mousePosF) && _isRunning)
            {
                _laps.push_back(_elapsedTime);
                updateLapDisplay();
            }
        }

        // Mode switch
        sf::FloatRect modeArea = this->_modeText.getGlobalBounds();
        modeArea.left -= 20.0f; modeArea.top -= 10.0f;
        modeArea.width += 40.0f; modeArea.height += 20.0f;

        if (modeArea.contains(mousePosF) && !_isRunning)
        {
            _elapsedTime = 0.0f;
            _laps.clear();
            _lapTexts.clear();
            _timerFinished = false;

            if (_mode == StopwatchMode::Stopwatch)
            {
                _mode = StopwatchMode::Timer;
                this->_modeLabel.setString("TIMER");
                this->_modeText.setString("STOPER");
                this->_lapText.setFillColor(sf::Color(100, 100, 100));
            }
            else
            {
                _mode = StopwatchMode::Stopwatch;
                this->_modeLabel.setString("STOPER");
                this->_modeText.setString("TIMER");
                this->_lapText.setFillColor(sf::Color::Yellow);
            }

            sf::FloatRect modeBounds = this->_modeLabel.getLocalBounds();
            this->_modeLabel.setOrigin(modeBounds.width / 2.0f, modeBounds.height / 2.0f);

            sf::FloatRect modeTextBounds = this->_modeText.getLocalBounds();
            this->_modeText.setOrigin(modeTextBounds.width / 2.0f, modeTextBounds.height / 2.0f);

            this->_timeDisplay.setString(formatTime(0.0f));
            this->_timeDisplay.setFillColor(sf::Color::White);
        }

        // Timer +/- buttons
        if (_mode == StopwatchMode::Timer && !_isRunning && _elapsedTime == 0.0f)
        {
            sf::FloatRect plusMinArea = this->_plusMinText.getGlobalBounds();
            plusMinArea.left -= 15.0f; plusMinArea.top -= 10.0f;
            plusMinArea.width += 30.0f; plusMinArea.height += 20.0f;
            if (plusMinArea.contains(mousePosF))
                _timerMinutes = min(_timerMinutes + 1, 99);

            sf::FloatRect minusMinArea = this->_minusMinText.getGlobalBounds();
            minusMinArea.left -= 15.0f; minusMinArea.top -= 10.0f;
            minusMinArea.width += 30.0f; minusMinArea.height += 20.0f;
            if (minusMinArea.contains(mousePosF))
                _timerMinutes = max(_timerMinutes - 1, 0);

            sf::FloatRect plusSecArea = this->_plusSecText.getGlobalBounds();
            plusSecArea.left -= 15.0f; plusSecArea.top -= 10.0f;
            plusSecArea.width += 30.0f; plusSecArea.height += 20.0f;
            if (plusSecArea.contains(mousePosF))
                _timerSeconds = (_timerSeconds + 5) % 60;

            sf::FloatRect minusSecArea = this->_minusSecText.getGlobalBounds();
            minusSecArea.left -= 15.0f; minusSecArea.top -= 10.0f;
            minusSecArea.width += 30.0f; minusSecArea.height += 20.0f;
            if (minusSecArea.contains(mousePosF))
                _timerSeconds = (_timerSeconds - 5 + 60) % 60;

            ostringstream oss;
            oss << std::setfill('0') << std::setw(2) << _timerMinutes << ":"
                << std::setfill('0') << std::setw(2) << _timerSeconds;
            this->_timerSettingText.setString(oss.str());
            sf::FloatRect tsBounds = this->_timerSettingText.getLocalBounds();
            this->_timerSettingText.setOrigin(tsBounds.width / 2.0f, tsBounds.height / 2.0f);
        }
    }
}

void StopwatchState::State_Update(float dt)
{
    if (this->_isRunning)
    {
        float delta = this->_clock.restart().asSeconds();

        if (_mode == StopwatchMode::Stopwatch)
        {
            this->_elapsedTime += delta;
        }
        else
        {
            this->_elapsedTime -= delta;
            if (this->_elapsedTime <= 0.0f)
            {
                this->_elapsedTime = 0.0f;
                this->_isRunning = false;
                this->_timerFinished = true;
                this->_startStopText.setString("START");
                this->_startStopText.setFillColor(sf::Color::Green);
                sf::FloatRect bounds = this->_startStopText.getLocalBounds();
                this->_startStopText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
            }
        }

        this->_timeDisplay.setString(formatTime(this->_elapsedTime));

        sf::FloatRect timeBounds = this->_timeDisplay.getLocalBounds();
        this->_timeDisplay.setOrigin(timeBounds.width / 2.0f, timeBounds.height / 2.0f);

        // Miganie gdy timer < 10s
        if (_mode == StopwatchMode::Timer && _elapsedTime < 10.0f && _elapsedTime > 0.0f)
        {
            int blink = static_cast<int>(_elapsedTime * 4) % 2;
            this->_timeDisplay.setFillColor(blink ? sf::Color::Red : sf::Color::White);
        }
    }
    else
    {
        this->_clock.restart();

        if (_timerFinished)
        {
            static float blinkTimer = 0.0f;
            blinkTimer += dt;
            int blink = static_cast<int>(blinkTimer * 3) % 2;
            this->_timeDisplay.setFillColor(blink ? sf::Color::Green : sf::Color::White);
        }
    }
}

void StopwatchState::State_Draw(float dt)
{
    this->_data->window.draw(this->_background);
    this->_data->window.draw(this->_modeLabel);
    this->_data->window.draw(this->_timeDisplay);
    this->_data->window.draw(this->_startStopText);
    this->_data->window.draw(this->_resetText);
    this->_data->window.draw(this->_lapText);
    this->_data->window.draw(this->_modeText);

    // Timer controls
    if (_mode == StopwatchMode::Timer && !_isRunning && _elapsedTime == 0.0f)
    {
        this->_data->window.draw(this->_timerSettingText);
        this->_data->window.draw(this->_plusMinText);
        this->_data->window.draw(this->_minusMinText);
        this->_data->window.draw(this->_plusSecText);
        this->_data->window.draw(this->_minusSecText);
    }

    // Lap times
    for (const auto& lapText : _lapTexts)
    {
        this->_data->window.draw(lapText);
    }
}