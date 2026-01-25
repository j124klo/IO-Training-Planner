#include "ProgressState.hpp"
#include <sstream>
#include <iomanip> 

using namespace std;

ProgressState::ProgressState(AppDataRef data) : GuiState(data)
{
}

void ProgressState::State_Init()
{
    this->_background.setTexture(this->_data->assets.getTexture("Default Background"));
    const sf::Font& font = this->_data->assets.getFont("Default Font");

    // 1. POBIERANIE DANYCH (4 Kategorie)
    if (this->_data->currentUser != nullptr) {
        int uid = this->_data->currentUser->id;
        this->_weightRecords = this->_data->database.getBestWeights(uid);
        this->_repsRecords = this->_data->database.getBestReps(uid);
        this->_timeRecords = this->_data->database.getBestTimes(uid);
        this->_distRecords = this->_data->database.getBestDistances(uid); // <--- NOWE
    }

    // 2. TYTU£
    _mainTitle.setFont(font);
    _mainTitle.setString("PERSONAL RECORDS (PR)");
    _mainTitle.setCharacterSize(40);
    _mainTitle.setFillColor(sf::Color::White);
    _mainTitle.setStyle(sf::Text::Bold);
    sf::FloatRect tr = _mainTitle.getLocalBounds();
    _mainTitle.setOrigin(tr.left + tr.width / 2.0f, tr.top + tr.height / 2.0f);
    _mainTitle.setPosition(600.0f, 40.0f);

    float currentY = 100.0f;

    // --- FUNKCJA RYSUJ¥CA TABELE ---
    // type: 0=Strength, 1=Endurance, 2=Time, 3=Distance
    auto createTable = [&](string title, const vector<PersonalRecord>& records, int type, sf::Color mainColor)
        {
            if (records.empty()) return;

            // Tytu³ Kategorii
            sf::Text catTitle;
            catTitle.setFont(font);
            catTitle.setString(title);
            catTitle.setCharacterSize(28);
            catTitle.setFillColor(mainColor);
            catTitle.setStyle(sf::Text::Bold | sf::Text::Underlined);
            catTitle.setPosition(250.0f, currentY);
            _allTexts.push_back(catTitle);
            currentY += 40.0f;

            // Pozycje Kolumn
            float x1 = 250.0f;
            float x2 = 550.0f;
            float x3 = 700.0f;
            float x4 = 850.0f;

            auto addTxt = [&](string s, float x, float y, int size, sf::Color c, bool bold = false) {
                sf::Text t; t.setFont(font); t.setString(s); t.setCharacterSize(size); t.setFillColor(c); t.setPosition(x, y);
                if (bold) t.setStyle(sf::Text::Bold);
                _allTexts.push_back(t);
                };

            // Nag³ówki (Logika dla 4 typów)
            addTxt("Exercise", x1, currentY, 22, sf::Color(180, 180, 180));

            if (type == 0) { // Strength
                addTxt("Weight (kg)", x2, currentY, 22, sf::Color(180, 180, 180));
                addTxt("Reps", x3, currentY, 22, sf::Color(180, 180, 180));
            }
            else if (type == 1) { // Endurance
                addTxt("Max Reps", x2, currentY, 22, sf::Color(180, 180, 180));
                addTxt("Load (kg)", x3, currentY, 22, sf::Color(180, 180, 180));
            }
            else if (type == 2) { // Time
                addTxt("Time (s)", x2, currentY, 22, sf::Color(180, 180, 180));
            }
            else { // Distance
                addTxt("Dist (km)", x2, currentY, 22, sf::Color(180, 180, 180));
            }

            addTxt("Date", x4, currentY, 22, sf::Color(180, 180, 180));
            currentY += 30.0f;

            // Wiersze
            for (const auto& rec : records) {
                addTxt(rec.exerciseName, x1, currentY, 24, sf::Color::White, true);

                string mainVal, extraVal;

                if (type == 0) { // Strength
                    std::stringstream ss; ss << rec.weight; mainVal = ss.str();
                    extraVal = to_string(rec.reps);
                }
                else if (type == 1) { // Endurance
                    mainVal = to_string(rec.reps);
                    std::stringstream ss; ss << rec.weight; extraVal = ss.str();
                }
                else { // Time & Distance (Shared Logic)
                    std::stringstream ss; ss << rec.weight; mainVal = ss.str();
                    extraVal = "";
                }

                addTxt(mainVal, x2 + 15.0f, currentY, 24, mainColor, true);

                if (!extraVal.empty()) {
                    addTxt(extraVal, x3 + 10.0f, currentY, 24, sf::Color(200, 255, 200));
                }

                addTxt(rec.date, x4, currentY + 5.0f, 18, sf::Color(120, 120, 120));
                currentY += 35.0f;
            }
            currentY += 30.0f;
        };

    // Generowanie 4 tabel
    createTable("MAX STRENGTH", _weightRecords, 0, sf::Color::Cyan);       // Si³a
    createTable("MAX ENDURANCE", _repsRecords, 1, sf::Color::Green);      // Wytrzyma³oœæ
    createTable("MAX TIME", _timeRecords, 2, sf::Color(255, 165, 0));     // Czas (Orange)
    createTable("MAX DISTANCE", _distRecords, 3, sf::Color::Magenta);     // Dystans (Magenta)
}

void ProgressState::State_HandleInput(sf::Event& event) {}
void ProgressState::State_Update(float dt) {}

void ProgressState::State_Draw(float dt)
{
    this->_data->window.draw(this->_background);
    this->_data->window.draw(this->_mainTitle);

    for (const auto& txt : _allTexts) {
        this->_data->window.draw(txt);
    }
}