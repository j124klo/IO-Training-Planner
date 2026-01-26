#include "DEFINITIONS.hpp"
#include "HomeState.hpp"
#include "TrainingState.hpp"
#include <iostream>

using namespace std;

HomeState::HomeState(AppDataRef data) : GuiState(data)
{

}

void HomeState::Buttons_Init()
{
    this->_data->assets.loadTexture("Start Training Button", PLACEHOLDER_LONG_BUTTON_FILEPATH);

	this->_startTrainingButton._sprite.setTexture(this->_data->assets.getTexture("Start Training Button"));

    this->_startTrainingButton._pressed = false;
    this->_startTrainingButton._update = false;
    this->_startTrainingButton._rect = sf::IntRect(0, 0, 800, 300);
    this->_startTrainingButton._sprite.setTextureRect(this->_startTrainingButton._rect);
    this->_startTrainingButton._sprite.setScale(0.35f, 0.35f);
    this->_startTrainingButton._sprite.setPosition(
        SCREAN_WIDTH / 2,   // - this->_startTrainingButton._sprite.getGlobalBounds().width / 2,
        SCREAN_HEIGHT / 2
    );
}
void HomeState::Buttons_HandleInput(sf::Event& event)
{
    if (this->_data->input.isSpriteClickedAndReleased(this->_startTrainingButton._sprite, sf::Mouse::Left, this->_data->window, this->_startTrainingButton._pressed, this->_startTrainingButton._update))
    {
        this->_data->machine.AddState(StateRef(new TrainingState(_data)), true);
    }
}
void HomeState::Buttons_Update(float dt)
{
    if (this->_startTrainingButton._update)
    {
        this->_startTrainingButton._update = false;
        this->_startTrainingButton._rect.left = this->_startTrainingButton._pressed ? 800 : 0;
        this->_startTrainingButton._sprite.setTextureRect(this->_startTrainingButton._rect);
    }
}
void HomeState::Buttons_Draw(float dt)
{
    this->_data->window.draw(this->_startTrainingButton._sprite);
}

void HomeState::ScrollPanel_Init()
{
    this->_data->assets.loadTexture("Scroll Edge", SCROLL_EDGE);
    this->_data->assets.loadTexture("Scroll Top", SCROLL_TOP);
    this->_data->assets.loadTexture("Scroll Mid", SCROLL_MID);
    this->_data->assets.loadTexture("Scroll Bot", SCROLL_BOT);
}

void HomeState::State_Init()
{
    // 1. £adowanie zasobów
    this->_data->assets.loadTexture("Default Background", DEFAULT_BACKGROUND_FILEPATH);
    this->_data->assets.loadTexture("Plan Panel", PLAN_PANEL_FILEPATH);

	ScrollPanel_Init();

    this->_background.setTexture(this->_data->assets.getTexture("Default Background"));
    this->_planPanel.setTexture(this->_data->assets.getTexture("Plan Panel"));

    this->_planPanel.setScale(0.6f, 0.6f);

    // Centrowanie ca³ego panelu na ekranie
    float panelX = 105.0f * 2 + (SCREAN_WIDTH - 105.0f * 2 - this->_planPanel.getGlobalBounds().width) / 2;
    float panelY = SCREAN_HEIGHT / 2 - this->_planPanel.getGlobalBounds().height / 2;

    this->_planPanel.setPosition(panelX, panelY);

    // --- 2. LOGIKA UK£ADANIA PLANU (CENTROWANIE) ---

    this->_planTexts.clear();

    if (this->_data->currentUser && this->_data->currentUser->currentPlanId.has_value()) {

        int planId = this->_data->currentUser->currentPlanId.value();
        auto dbPlan = this->_data->database.getTrainingPlanById(planId);

        if (dbPlan) {

            // === KONFIGURACJA WYMIARÓW (TU ZMIENIASZ LICZBY) ===

            float panelW = this->_planPanel.getGlobalBounds().width;

            // Ustawienia siatki
            float marginLeft = 0.0f;     // Przesuniêcie ca³ej siatki w prawo/lewo (gdyby nie trafia³a w kolumny)
            float colWidth = panelW / 7.0f; // Szerokoœæ jednej kolumny (automatyczna)

            // Ustawienia pionowe (Y)
            float headerHeight = 45.0f;  // Ile miejsca zajmuje nag³ówek "Monday" itp.
            float startY = panelY + headerHeight + 35.0f; // Punkt startowy dla pierwszego æwiczenia
            float rowHeight = 40.0f;     // Sta³y odstêp miêdzy liniami (im wiêcej, tym rzadziej)

            // Wygl¹d tekstu
            int fontSize = 22;           // Rozmiar czcionki (¿eby siê mieœci³o)

            for (const auto& entry : dbPlan->entries) {

                // Pobieranie nazwy
                string exName = "Unknown";
                auto ex = this->_data->database.getExerciseById(entry.exerciseId);
                if (ex) exName = ex->name;

                // Konfiguracja SFML Text
                sf::Text text;
                text.setFont(this->_data->assets.getFont("Default Font"));
                text.setString(exName);
                text.setCharacterSize(fontSize);
                text.setFillColor(sf::Color(77, 153, 204));

                // === MAGIA CENTROWANIA POZIOMEGO (X) ===

                // 1. ZnajdŸ lew¹ krawêdŸ kolumny dla danego dnia
                int colIndex = entry.dayOfWeek - 1; // 0..6
                float colLeftEdge = panelX + marginLeft + (colIndex * colWidth);

                // 2. ZnajdŸ œrodek tej kolumny
                float colCenter = colLeftEdge + (colWidth / 2.0f);

                // 3. Pobierz szerokoœæ samego napisu
                sf::FloatRect textBounds = text.getLocalBounds();

                // 4. Ustaw X tak, ¿eby œrodek napisu pokrywa³ siê ze œrodkiem kolumny
                // (Odejmujemy po³owê szerokoœci napisu od œrodka kolumny)
                float xPos = colCenter - (textBounds.width / 2.0f);


                // === USTAWIANIE PIONOWE (Y) ===

                // entry.orderIndex to kolejnoœæ w dniu (1, 2, 3...)
                // Odejmujemy 1, ¿eby pierwszy element by³ na górze (index 0)
                int rowIndex = entry.orderInDay - 1;

                float yPos = startY + (rowIndex * rowHeight);

                // Finalne ustawienie
                text.setPosition(xPos, yPos);

                // Dodajemy do listy rysowania
                this->_planTexts.push_back(text);
            }
        }
    }
}
void HomeState::State_HandleInput(sf::Event& event)
{
	Buttons_HandleInput(event);
}
void HomeState::State_Update(float dt)
{
	Buttons_Update(dt);
}
void HomeState::State_Draw(float dt)
{
    this->_data->window.draw(this->_background);

    this->_data->window.draw(this->_planPanel);
    for (const auto& text : this->_planTexts) {
        this->_data->window.draw(text);
    }

	Buttons_Draw(dt);
}