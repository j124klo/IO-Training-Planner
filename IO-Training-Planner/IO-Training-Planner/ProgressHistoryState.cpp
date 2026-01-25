#include "DEFINITIONS.hpp"
#include "ProgressHistoryState.hpp"
#include "ProgressState.hpp"
#include <iomanip>

using namespace std;

ProgressHistoryState::ProgressHistoryState(AppDataRef data) : GuiState(data)
{
}

void ProgressHistoryState::State_Init()
{
    this->_background.setTexture(this->_data->assets.getTexture("Default Background"));
    _font = this->_data->assets.getFont("Default Font");

    // 1. Przycisk Wstecz - PRZESUNIÊTY NA PRAW¥ STRONÊ
    _backButtonShape.setSize(sf::Vector2f(100.0f, 40.0f));
    _backButtonShape.setFillColor(sf::Color(100, 100, 100)); // Szary
    _backButtonShape.setPosition(SCREAN_WIDTH - 150.0f, 40.0f); // Prawy górny róg

    _backButtonLabel.setFont(_font);
    _backButtonLabel.setString("Back");
    _backButtonLabel.setCharacterSize(20);
    _backButtonLabel.setFillColor(sf::Color::White);
    // Centrowanie napisu na przycisku
    _backButtonLabel.setPosition(SCREAN_WIDTH - 135.0f, 48.0f);

    // 2. Tytu³ - Przesuniêty bardziej w prawo
    _title.setFont(_font);
    _title.setString("TRAINING HISTORY");
    _title.setCharacterSize(35);
    _title.setFillColor(sf::Color::White);
    _title.setPosition(400.0f, 40.0f); // X=400, ¿eby omin¹æ menu

    // 3. Pobranie danych
    if (this->_data->currentUser) {
        this->_logs = this->_data->database.getWorkoutLogs(this->_data->currentUser->id);
    }

    // 4. Budowanie tabeli - POPRAWIONE POZYCJE X (odsuniête od lewego menu)
    float currentY = 120.0f;

    // Przesuwamy wszystko o ok. 80-100 pikseli w prawo wzglêdem starej wersji
    float colDate = 280.0f;  // By³o 200
    float colName = 480.0f;  // By³o 400
    float colSet = 750.0f;   // By³o 700
    float colRes = 850.0f;   // By³o 800

    auto addText = [&](string s, float x, float y, sf::Color c, int size = 20, bool bold = false) {
        sf::Text t;
        t.setFont(_font);
        t.setString(s);
        t.setCharacterSize(size);
        t.setFillColor(c);
        t.setPosition(x, y);
        if (bold) t.setStyle(sf::Text::Bold);
        _logTexts.push_back(t);
        };

    // Nag³ówki
    addText("Date", colDate, currentY, sf::Color::Cyan, 22, true);
    addText("Exercise", colName, currentY, sf::Color::Cyan, 22, true);
    addText("Set", colSet, currentY, sf::Color::Cyan, 22, true);
    addText("Result", colRes, currentY, sf::Color::Cyan, 22, true);

    currentY += 40.0f;

    // Wiersze
    for (const auto& log : _logs) {
        // Data
        addText(log.date, colDate, currentY, sf::Color(200, 200, 200));

        // Nazwa
        addText(log.exerciseNameVal, colName, currentY, sf::Color::White);

        // Numer serii (centrujemy lekko)
        addText(to_string(log.setNumber), colSet + 10.0f, currentY, sf::Color(150, 150, 150));

        // Wynik (Inteligentne formatowanie)
        string resultStr = formatLogResult(log);
        addText(resultStr, colRes, currentY, sf::Color::Green);

        currentY += 30.0f;
    }
}

string ProgressHistoryState::formatLogResult(const WorkoutLog& log)
{
    // Pobieramy æwiczenie, ¿eby znaæ typ
    auto ex = this->_data->database.getExerciseById(log.exerciseId);

    if (!ex) {
        stringstream ss;
        ss << log.weight << " - " << log.reps;
        return ss.str();
    }

    bool hasDist = false;
    bool hasTime = false;
    for (auto vt : ex->valueTypes) {
        if (vt == ValueType::DISTANCE_KM) hasDist = true;
        if (vt == ValueType::TIME_SEC) hasTime = true;
    }

    stringstream ss;
    if (hasDist) {
        // Bieganie: km / min
        ss << log.weight << "km / " << log.reps << "min";
    }
    else if (hasTime) {
        // Plank: s
        ss << log.weight << "s";
    }
    else {
        // Si³ownia
        if (log.weight > 0) {
            ss << log.weight << "kg x " << log.reps;
        }
        else {
            // Jeœli ciê¿ar 0 (np. pompki), wyœwietl tylko powtórzenia
            ss << log.reps << " reps";
        }
    }

    return ss.str();
}

void ProgressHistoryState::State_HandleInput(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(this->_data->window);

        // Obs³uga przycisku Wstecz
        if (_backButtonShape.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
            // Wracamy do ProgressState
            this->_data->machine.AddState(StateRef(new ProgressState(this->_data)), true);
        }
    }
}

void ProgressHistoryState::State_Update(float dt)
{
    // Hover effect
    sf::Vector2i mousePos = sf::Mouse::getPosition(this->_data->window);
    if (_backButtonShape.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
        _backButtonShape.setOutlineColor(sf::Color::White);
        _backButtonShape.setOutlineThickness(1.0f);
    }
    else {
        _backButtonShape.setOutlineThickness(0.0f);
    }
}

void ProgressHistoryState::State_Draw(float dt)
{
    this->_data->window.draw(this->_background);
    this->_data->window.draw(this->_title);

    this->_data->window.draw(this->_backButtonShape);
    this->_data->window.draw(this->_backButtonLabel);

    for (const auto& txt : _logTexts) {
        this->_data->window.draw(txt);
    }
}