#include "DEFINITIONS.hpp"
#include "StopwatchState.hpp"

using namespace std;

StopwatchState::StopwatchState(AppDataRef data) : GuiState(data)
{
    _currentTime = 0.0f;
    _isRunning = false;
    _phase = IntervalPhase::Prepare;
    
    // Default settings
    _workSeconds = 30;
    _restSeconds = 10;
    _prepareSeconds = 5;
    _totalRounds = 8;
    _currentRound = 1;
}

string StopwatchState::formatTime(float seconds)
{
    int mins = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    int ms = static_cast<int>((seconds - static_cast<int>(seconds)) * 10);

    ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << mins << ":"
        << std::setfill('0') << std::setw(2) << secs << "."
        << ms;

    return oss.str();
}

string StopwatchState::formatTimeShort(int seconds)
{
    int mins = seconds / 60;
    int secs = seconds % 60;

    ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << mins << ":"
        << std::setfill('0') << std::setw(2) << secs;

    return oss.str();
}

void StopwatchState::setupText(sf::Text& text, const string& label, int size, float x, float y, sf::Color color)
{
    text.setFont(this->_data->assets.getFont("Default Font"));
    text.setString(label);
    text.setCharacterSize(size);
    text.setFillColor(color);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    text.setPosition(x, y);
}

void StopwatchState::updateSettingsDisplay()
{
    this->_workTimeText.setString(formatTimeShort(_workSeconds));
    sf::FloatRect wb = this->_workTimeText.getLocalBounds();
    this->_workTimeText.setOrigin(wb.width / 2.0f, wb.height / 2.0f);

    this->_restTimeText.setString(formatTimeShort(_restSeconds));
    sf::FloatRect rb = this->_restTimeText.getLocalBounds();
    this->_restTimeText.setOrigin(rb.width / 2.0f, rb.height / 2.0f);

    this->_roundsText.setString(to_string(_totalRounds));
    sf::FloatRect rndb = this->_roundsText.getLocalBounds();
    this->_roundsText.setOrigin(rndb.width / 2.0f, rndb.height / 2.0f);
}

float StopwatchState::getCurrentPhaseDuration()
{
    switch (_phase)
    {
        case IntervalPhase::Prepare: return static_cast<float>(_prepareSeconds);
        case IntervalPhase::Work: return static_cast<float>(_workSeconds);
        case IntervalPhase::Rest: return static_cast<float>(_restSeconds);
        default: return 0.0f;
    }
}

void StopwatchState::nextPhase()
{
    switch (_phase)
    {
        case IntervalPhase::Prepare:
            _phase = IntervalPhase::Work;
            _currentTime = static_cast<float>(_workSeconds);
            break;

        case IntervalPhase::Work:
            if (_currentRound >= _totalRounds)
            {
                _phase = IntervalPhase::Finished;
                _isRunning = false;
                _currentTime = 0.0f;
            }
            else
            {
                _phase = IntervalPhase::Rest;
                _currentTime = static_cast<float>(_restSeconds);
            }
            break;

        case IntervalPhase::Rest:
            _currentRound++;
            _phase = IntervalPhase::Work;
            _currentTime = static_cast<float>(_workSeconds);
            break;

        default:
            break;
    }
}

void StopwatchState::State_Init()
{
    this->_background.setTexture(this->_data->assets.getTexture("Default Background"));

    float centerX = SCREAN_WIDTH / 2.0f;

    // Main time display
    setupText(this->_timeDisplay, "00:05.0", 96, centerX, SCREAN_HEIGHT / 4.0f);
    _currentTime = static_cast<float>(_prepareSeconds);

    // Phase label
    setupText(this->_phaseLabel, "GET READY", 36, centerX, SCREAN_HEIGHT / 4.0f - 80.0f, sf::Color::Yellow);

    // Round label
    setupText(this->_roundLabel, "Round 1 / 8", 28, centerX, SCREAN_HEIGHT / 4.0f + 70.0f, sf::Color::Cyan);

    // Progress bar
    float barWidth = 500.0f;
    float barHeight = 20.0f;
    float barY = SCREAN_HEIGHT / 4.0f + 110.0f;

    _progressBarBg.setSize(sf::Vector2f(barWidth, barHeight));
    _progressBarBg.setFillColor(sf::Color(60, 60, 60));
    _progressBarBg.setPosition(centerX - barWidth / 2.0f, barY);

    _progressBarFill.setSize(sf::Vector2f(barWidth, barHeight));
    _progressBarFill.setFillColor(sf::Color::Yellow);
    _progressBarFill.setPosition(centerX - barWidth / 2.0f, barY);

    // Main buttons
    float buttonY = SCREAN_HEIGHT / 2.0f + 20.0f;
    setupText(this->_startStopText, "START", 32, centerX - 100.0f, buttonY, sf::Color::Green);
    setupText(this->_resetText, "RESET", 32, centerX + 100.0f, buttonY);

    // === SETTINGS SECTION ===
    float settingsY = SCREAN_HEIGHT / 2.0f + 120.0f;
    float col1 = centerX - 200.0f;
    float col2 = centerX;
    float col3 = centerX + 200.0f;

    // Work
    setupText(this->_workLabel, "WORK", 20, col1, settingsY - 40.0f, sf::Color(100, 255, 100));
    setupText(this->_workTimeText, formatTimeShort(_workSeconds), 28, col1, settingsY);
    setupText(this->_workPlusText, "+", 28, col1 + 50.0f, settingsY);
    setupText(this->_workMinusText, "-", 28, col1 - 50.0f, settingsY);

    // Rest
    setupText(this->_restLabel, "REST", 20, col2, settingsY - 40.0f, sf::Color(100, 100, 255));
    setupText(this->_restTimeText, formatTimeShort(_restSeconds), 28, col2, settingsY);
    setupText(this->_restPlusText, "+", 28, col2 + 50.0f, settingsY);
    setupText(this->_restMinusText, "-", 28, col2 - 50.0f, settingsY);

    // Rounds
    setupText(this->_roundsLabel, "ROUNDS", 20, col3, settingsY - 40.0f, sf::Color(255, 200, 100));
    setupText(this->_roundsText, to_string(_totalRounds), 28, col3, settingsY);
    setupText(this->_roundsPlusText, "+", 28, col3 + 40.0f, settingsY);
    setupText(this->_roundsMinusText, "-", 28, col3 - 40.0f, settingsY);

    this->_clock.restart();
}

void StopwatchState::State_HandleInput(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(this->_data->window).x),
                              static_cast<float>(sf::Mouse::getPosition(this->_data->window).y));

        auto isClicked = [&](sf::Text& text) {
            sf::FloatRect area = text.getGlobalBounds();
            area.left -= 15.0f; area.top -= 10.0f;
            area.width += 30.0f; area.height += 20.0f;
            return area.contains(mousePos);
        };

        // Start/Stop
        if (isClicked(this->_startStopText))
        {
            if (_phase == IntervalPhase::Finished)
            {
                // Reset after completion
                _phase = IntervalPhase::Prepare;
                _currentRound = 1;
                _currentTime = static_cast<float>(_prepareSeconds);
            }

            this->_isRunning = !this->_isRunning;

            if (this->_isRunning)
            {
                this->_startStopText.setString("PAUSE");
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
        if (isClicked(this->_resetText))
        {
            this->_isRunning = false;
            this->_phase = IntervalPhase::Prepare;
            this->_currentRound = 1;
            this->_currentTime = static_cast<float>(_prepareSeconds);

            this->_startStopText.setString("START");
            this->_startStopText.setFillColor(sf::Color::Green);
            sf::FloatRect bounds = this->_startStopText.getLocalBounds();
            this->_startStopText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

            this->_clock.restart();
        }

        // Settings (only when stopped and in prepare phase)
        if (!_isRunning && _phase == IntervalPhase::Prepare)
        {
            // Work +/-
            if (isClicked(this->_workPlusText))
                _workSeconds = min(_workSeconds + 5, 300);
            if (isClicked(this->_workMinusText))
                _workSeconds = max(_workSeconds - 5, 5);

            // Rest +/-
            if (isClicked(this->_restPlusText))
                _restSeconds = min(_restSeconds + 5, 120);
            if (isClicked(this->_restMinusText))
                _restSeconds = max(_restSeconds - 5, 5);

            // Rounds +/-
            if (isClicked(this->_roundsPlusText))
                _totalRounds = min(_totalRounds + 1, 50);
            if (isClicked(this->_roundsMinusText))
                _totalRounds = max(_totalRounds - 1, 1);

            updateSettingsDisplay();
        }
    }
}

void StopwatchState::State_Update(float dt)
{
    // Update round label
    ostringstream roundOss;
    roundOss << "Round " << _currentRound << " / " << _totalRounds;
    this->_roundLabel.setString(roundOss.str());
    sf::FloatRect rlb = this->_roundLabel.getLocalBounds();
    this->_roundLabel.setOrigin(rlb.width / 2.0f, rlb.height / 2.0f);

    // Update phase label and color
    switch (_phase)
    {
        case IntervalPhase::Prepare:
            this->_phaseLabel.setString("GET READY");
            this->_phaseLabel.setFillColor(sf::Color::Yellow);
            this->_progressBarFill.setFillColor(sf::Color::Yellow);
            break;
        case IntervalPhase::Work:
            this->_phaseLabel.setString("WORK!");
            this->_phaseLabel.setFillColor(sf::Color::Green);
            this->_progressBarFill.setFillColor(sf::Color::Green);
            break;
        case IntervalPhase::Rest:
            this->_phaseLabel.setString("REST");
            this->_phaseLabel.setFillColor(sf::Color::Cyan);
            this->_progressBarFill.setFillColor(sf::Color::Cyan);
            break;
        case IntervalPhase::Finished:
            this->_phaseLabel.setString("FINISHED!");
            this->_phaseLabel.setFillColor(sf::Color::Magenta);
            this->_progressBarFill.setFillColor(sf::Color::Magenta);
            break;
    }
    sf::FloatRect plb = this->_phaseLabel.getLocalBounds();
    this->_phaseLabel.setOrigin(plb.width / 2.0f, plb.height / 2.0f);

    if (this->_isRunning)
    {
        float delta = this->_clock.restart().asSeconds();
        this->_currentTime -= delta;

        if (this->_currentTime <= 0.0f)
        {
            nextPhase();
        }

        // Blinking when < 3 seconds
        if (_currentTime <= 3.0f && _currentTime > 0.0f && _phase != IntervalPhase::Finished)
        {
            int blink = static_cast<int>(_currentTime * 4) % 2;
            this->_timeDisplay.setFillColor(blink ? sf::Color::Red : sf::Color::White);
        }
        else
        {
            this->_timeDisplay.setFillColor(sf::Color::White);
        }
    }
    else
    {
        this->_clock.restart();
        this->_timeDisplay.setFillColor(sf::Color::White);
    }

    // Update time display
    this->_timeDisplay.setString(formatTime(max(0.0f, _currentTime)));
    sf::FloatRect timeBounds = this->_timeDisplay.getLocalBounds();
    this->_timeDisplay.setOrigin(timeBounds.width / 2.0f, timeBounds.height / 2.0f);

    // Progress bar
    float phaseDuration = getCurrentPhaseDuration();
    float progress = (phaseDuration > 0.0f) ? (_currentTime / phaseDuration) : 0.0f;
    progress = max(0.0f, min(1.0f, progress));
    this->_progressBarFill.setSize(sf::Vector2f(500.0f * progress, 20.0f));
}

void StopwatchState::State_Draw(float dt)
{
    this->_data->window.draw(this->_background);

    // Main elements
    this->_data->window.draw(this->_phaseLabel);
    this->_data->window.draw(this->_timeDisplay);
    this->_data->window.draw(this->_roundLabel);

    // Progress bar
    this->_data->window.draw(this->_progressBarBg);
    this->_data->window.draw(this->_progressBarFill);

    // Buttons
    this->_data->window.draw(this->_startStopText);
    this->_data->window.draw(this->_resetText);

    // Settings (visible when stopped and in prepare phase)
    if (!_isRunning && _phase == IntervalPhase::Prepare)
    {
        this->_data->window.draw(this->_workLabel);
        this->_data->window.draw(this->_workTimeText);
        this->_data->window.draw(this->_workPlusText);
        this->_data->window.draw(this->_workMinusText);

        this->_data->window.draw(this->_restLabel);
        this->_data->window.draw(this->_restTimeText);
        this->_data->window.draw(this->_restPlusText);
        this->_data->window.draw(this->_restMinusText);

        this->_data->window.draw(this->_roundsLabel);
        this->_data->window.draw(this->_roundsText);
        this->_data->window.draw(this->_roundsPlusText);
        this->_data->window.draw(this->_roundsMinusText);
    }
}