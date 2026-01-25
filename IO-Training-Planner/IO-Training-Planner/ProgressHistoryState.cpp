#include "DEFINITIONS.hpp"
#include "ProgressHistoryState.hpp"
#include "ProgressState.hpp"
#include <iomanip> // Do formatowania dat/liczb

using namespace std;

ProgressHistoryState::ProgressHistoryState(AppDataRef data) : GuiState(data)
{
}

void ProgressHistoryState::ProgressButton_Init()
{
    // NAPRAWA B£ÊDU: Najpierw ³adujemy teksturê, potem jej u¿ywamy.
    // U¿ywamy PLACEHOLDER_SHORT_BUTTON_FILEPATH lub innej grafiki dla przycisku "Wstecz"
    this->_data->assets.loadTexture("Back Button", PLACEHOLDER_SHORT_BUTTON_FILEPATH);
    this->_backToProgressButton._sprite.setTexture(this->_data->assets.getTexture("Back Button"));

    // Ustawiamy pozycjê (lewy dolny róg, tak jak w menu)
    float menuButtonWidth;
    float menuButtonHeight;
    sf::Vector2f menuButtonPosition = GetMenuButtonPosition(menuButtonWidth, menuButtonHeight);

    this->_backToProgressButton._pressed = false;
    this->_backToProgressButton._update = false;
    this->_backToProgressButton._rect = sf::IntRect(0, 0, 300, 300);
    this->_backToProgressButton._sprite.setTextureRect(this->_backToProgressButton._rect);
    this->_backToProgressButton._sprite.setScale(0.35f, 0.35f);

    // Pozycja przycisku powrotu (np. lewy dolny róg)
    this->_backToProgressButton._sprite.setPosition(menuButtonPosition.x, SCREAN_HEIGHT - this->_backToProgressButton._sprite.getGlobalBounds().height - 20.0f);
}

void ProgressHistoryState::ProgressButton_HandleInput(sf::Event& event)
{
    if (this->_data->input.isSpriteClickedAndReleased(this->_backToProgressButton._sprite, sf::Mouse::Left, this->_data->window, this->_backToProgressButton._pressed, this->_backToProgressButton._update))
    {
        // Wracamy do ProgressState (zastêpujemy obecny stan)
        this->_data->machine.AddState(StateRef(new ProgressState(this->_data)), true);
    }
}

void ProgressHistoryState::ProgressButton_Update(float dt)
{
    if (this->_backToProgressButton._update)
    {
        this->_backToProgressButton._update = false;
        if (this->_backToProgressButton._pressed) {
            this->_backToProgressButton._rect.left = 300;
        }
        else {
            this->_backToProgressButton._rect.left = 0;
        }
        this->_backToProgressButton._sprite.setTextureRect(this->_backToProgressButton._rect);
    }
}

void ProgressHistoryState::ProgressButton_Draw(float dt)
{
    this->_data->window.draw(this->_backToProgressButton._sprite);
}

void ProgressHistoryState::State_Init()
{
    this->_background.setTexture(this->_data->assets.getTexture("Default Background"));

    // Inicjalizacja przycisku powrotu
    ProgressButton_Init();

    // --- WYŒWIETLANIE HISTORII ---
    const sf::Font& font = this->_data->assets.getFont("Default Font");

    // 1. Tytu³
    _title.setFont(font);
    _title.setString("PELNA HISTORIA TRENINGOW");
    _title.setCharacterSize(35);
    _title.setFillColor(sf::Color::White);
    _title.setPosition(300.0f, 30.0f); // Przesuniête w prawo, ¿eby nie wchodzi³o na menu

    // 2. Pobranie danych
    if (this->_data->currentUser) {
        // U¿ywamy metody getWorkoutLogs, któr¹ stworzyliœmy wczeœniej
        this->_logs = this->_data->database.getWorkoutLogs(this->_data->currentUser->id);
    }

    // 3. Generowanie listy (prosta tabela na start)
    float currentY = 100.0f;
    float startX = 280.0f; // ¯eby omin¹æ pasek boczny

    // Nag³ówki
    auto addText = [&](string s, float x, float y, sf::Color c, int size = 20) {
        sf::Text t;
        t.setFont(font);
        t.setString(s);
        t.setCharacterSize(size);
        t.setFillColor(c);
        t.setPosition(x, y);
        _logTexts.push_back(t);
        };

    addText("Data", startX, currentY, sf::Color::Yellow);
    addText("Cwiczenie", startX + 150, currentY, sf::Color::Yellow);
    addText("Set", startX + 450, currentY, sf::Color::Yellow);
    addText("Wynik", startX + 520, currentY, sf::Color::Yellow);

    currentY += 40.0f;

    // Wiersze
    for (const auto& log : _logs) {
        // Jeœli lista jest bardzo d³uga, wyjdzie poza ekran (na razie bez scrolla)

        addText(log.date, startX, currentY, sf::Color(200, 200, 200));
        addText(log.exerciseNameVal, startX + 150, currentY, sf::Color::White); // Nazwa z JOINa
        addText(to_string(log.setNumber), startX + 460, currentY, sf::Color::Cyan);

        // Formatowanie wyniku: "100kg x 5" lub "Czas: 60s"
        stringstream ss;
        if (log.weight > 0) ss << log.weight << "kg x " << log.reps;
        else ss << log.reps << " reps"; // Dla pompek itp.

        addText(ss.str(), startX + 520, currentY, sf::Color::Green);

        currentY += 30.0f;
    }
}

void ProgressHistoryState::State_HandleInput(sf::Event& event)
{
    ProgressButton_HandleInput(event);
}

void ProgressHistoryState::State_Update(float dt)
{
    ProgressButton_Update(dt);
}

void ProgressHistoryState::State_Draw(float dt)
{
    this->_data->window.draw(this->_background);
    this->_data->window.draw(this->_title);

    // Rysowanie tabeli
    for (const auto& txt : _logTexts) {
        this->_data->window.draw(txt);
    }

    ProgressButton_Draw(dt);
}