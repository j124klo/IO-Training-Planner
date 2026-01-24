#include "ProgressState.hpp"
#include <sstream>
#include <iomanip> 

using namespace std;

ProgressState::ProgressState(AppDataRef data) : GuiState(data)
{
}

void ProgressState::State_Init()
{
    // T³o
    this->_background.setTexture(this->_data->assets.getTexture("Default Background"));

    // 1. Pobieranie REKORDÓW z bazy
    if (this->_data->currentUser != nullptr) {
        // U¿ywamy nowej metody getPersonalRecords
        this->_records = this->_data->database.getPersonalRecords(this->_data->currentUser->id);
    }

    const sf::Font& font = this->_data->assets.getFont("Default Font");

    // 2. Tytu³
    _titleText.setFont(font);
    _titleText.setString("MOJE REKORDY (PR)");
    _titleText.setCharacterSize(45);
    _titleText.setFillColor(sf::Color::White);
    _titleText.setStyle(sf::Text::Bold); // Pogrubienie tytu³u

    // Centrowanie (uwzglêdniaj¹c przesuniêcie w prawo, ¿eby nie wchodziæ na menu)
    sf::FloatRect textRect = _titleText.getLocalBounds();
    _titleText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    // Przesuwamy œrodek na X=600 (trochê w prawo od œrodka ekranu)
    _titleText.setPosition(600.0f, 60.0f);

    // 3. Nag³ówki tabeli
    float headerY = 140.0f;
    int charSize = 26;
    sf::Color headerColor = sf::Color(255, 215, 0); // Z³oty

    // Przesuwamy start tabeli na X=250, ¿eby omin¹æ pasek boczny z przyciskami
    float col1_X = 250.0f; // Nazwa
    float col2_X = 550.0f; // Max kg
    float col3_X = 720.0f; // Max Reps (NOWA)
    float col4_X = 900.0f; // Data

    auto setupHeader = [&](sf::Text& txt, string str, float x) {
        txt.setFont(font);
        txt.setString(str);
        txt.setCharacterSize(charSize);
        txt.setFillColor(headerColor);
        txt.setPosition(x, headerY);
        txt.setStyle(sf::Text::Bold);
        };

    setupHeader(_headerExercise, "Cwiczenie", col1_X);
    setupHeader(_headerMaxWeight, "Max (kg)", col2_X);
    setupHeader(_headerMaxReps, "Max Reps", col3_X);
    setupHeader(_headerDate, "Data", col4_X);

    // 4. Generowanie wierszy z rekordami
    float startY = 200.0f;
    float stepY = 60.0f; // Wiêkszy odstêp miêdzy wierszami dla czytelnoœci

    for (size_t i = 0; i < _records.size(); i++)
    {
        const auto& rec = _records[i];
        float rowY = startY + (static_cast<float>(i) * stepY);

        // --- Kolumna 1: Nazwa æwiczenia ---
        sf::Text txtName;
        txtName.setFont(font);
        txtName.setString(rec.exerciseName);
        txtName.setCharacterSize(30);
        txtName.setFillColor(sf::Color::White);
        txtName.setStyle(sf::Text::Bold);
        txtName.setPosition(col1_X, rowY);
        _logTexts.push_back(txtName);

        // --- Kolumna 2: Ciê¿ar (Cyan) ---
        sf::Text txtWeight;
        txtWeight.setFont(font);
        std::stringstream ssW;
        ssW << rec.maxWeight;
        txtWeight.setString(ssW.str());
        txtWeight.setCharacterSize(30);
        txtWeight.setFillColor(sf::Color::Cyan);
        txtWeight.setStyle(sf::Text::Bold); // Pogrubione
        txtWeight.setPosition(col2_X + 20.0f, rowY);
        _logTexts.push_back(txtWeight);

        // --- Kolumna 3: Powtórzenia (Lime/Zielony) ---
        sf::Text txtReps;
        txtReps.setFont(font);
        txtReps.setString(to_string(rec.maxReps));
        txtReps.setCharacterSize(30);
        txtReps.setFillColor(sf::Color::Green); // Zielony dla repsów
        txtReps.setStyle(sf::Text::Bold);       // Pogrubione
        txtReps.setPosition(col3_X + 30.0f, rowY);
        _logTexts.push_back(txtReps);

        // --- Kolumna 4: Data (Szara) ---
        sf::Text txtDate;
        txtDate.setFont(font);
        txtDate.setString(rec.date);
        txtDate.setCharacterSize(20);
        txtDate.setFillColor(sf::Color(180, 180, 180));
        txtDate.setPosition(col4_X, rowY + 8.0f);
        _logTexts.push_back(txtDate);
    }
}


void ProgressState::State_HandleInput(sf::Event& event)
{
}

void ProgressState::State_Update(float dt)
{
}

void ProgressState::State_Draw(float dt)
{
    this->_data->window.draw(this->_background);
    this->_data->window.draw(this->_titleText);

    this->_data->window.draw(_headerExercise);
    this->_data->window.draw(_headerMaxWeight);
    this->_data->window.draw(_headerMaxReps);
    this->_data->window.draw(_headerDate);

    for (const auto& txt : _logTexts) {
        this->_data->window.draw(txt);
    }
}