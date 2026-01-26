#include "DEFINITIONS.hpp"
#include "HomeState.hpp"
#include "TrainingState.hpp"

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
        this->_scrollTop.getPosition().x + this->_scrollTop.getGlobalBounds().width / 2 - this->_startTrainingButton._sprite.getGlobalBounds().width / 2,
		this->_scrollTop.getPosition().y + this->_scrollTop.getGlobalBounds().height
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

void HomeState::PlanPanel_Init()
{
    this->_data->assets.loadTexture("Plan Panel", PLAN_PANEL_FILEPATH);

    this->_planPanel.setTexture(this->_data->assets.getTexture("Plan Panel"));

    this->_planPanel.setScale(0.55f, 0.55f);

    float panelX = this->_scrollTop.getPosition().x + this->_scrollTop.getGlobalBounds().width / 2 - this->_planPanel.getGlobalBounds().width / 2;
    float panelY = this->_startTrainingButton._sprite.getPosition().y + this->_startTrainingButton._sprite.getGlobalBounds().height + 40.0f;
    this->_planPanel.setPosition(panelX, panelY);

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
void HomeState::PlanPanel_HandleInput(sf::Event& event)
{

}
void HomeState::PlanPanel_Update(float dt)
{

}
void HomeState::PlanPanel_Draw(float dt)
{
    this->_data->window.draw(this->_planPanel);
    for (const auto& text : this->_planTexts) {
        this->_data->window.draw(text);
    }
}

void HomeState::ScrollPanel_Init()
{
    this->_data->assets.loadTexture("Scroll Edge", SCROLL_EDGE);
    this->_data->assets.loadTexture("Scroll Top", SCROLL_TOP);
    this->_data->assets.loadTexture("Scroll Mid", SCROLL_MID);
    this->_data->assets.loadTexture("Scroll Bot", SCROLL_BOT);

    this->_scrollEdgeTop.setTexture(this->_data->assets.getTexture("Scroll Edge"));
    this->_scrollEdgeBot.setTexture(this->_data->assets.getTexture("Scroll Edge"));
    this->_scrollTop.setTexture(this->_data->assets.getTexture("Scroll Top"));
    this->_scrollMid.setTexture(this->_data->assets.getTexture("Scroll Mid"));
    this->_scrollBot.setTexture(this->_data->assets.getTexture("Scroll Bot"));

    float menuButtonWidth;
    float menuButtonHeight;
    sf::Vector2f menuButtonPosition = GetMenuButtonPosition(menuButtonWidth, menuButtonHeight);

    this->_scrollEdgeTop.setPosition(2 * menuButtonWidth + (SCREAN_WIDTH - 2 * menuButtonWidth - this->_scrollEdgeTop.getGlobalBounds().width) / 2, 0.0f);
    this->_scrollEdgeBot.setPosition(this->_scrollEdgeTop.getPosition().x, SCREAN_HEIGHT - this->_scrollEdgeBot.getGlobalBounds().height);

    this->_scrollTop.setPosition(this->_scrollEdgeTop.getPosition().x, this->_scrollEdgeTop.getGlobalBounds().height - this->_scrollTop.getGlobalBounds().height / 2.0f);
    this->_scrollTopDestination = this->_scrollTop.getPosition();

    this->_scrollMid.setPosition(this->_scrollEdgeTop.getPosition().x, 0.0f);
}
void HomeState::ScrollPanel_HandleInput(sf::Event& event)
{
    this->_data->input.handleScrollEvent(event);
}
void HomeState::ScrollPanel_Update(float dt)
{
    float scrollDelta = this->_data->input.getScrollDelta() * 80.0f;

    float topBound = this->_scrollEdgeTop.getGlobalBounds().height - this->_scrollTop.getGlobalBounds().height / 2.0f;

    float contentDeltaY = this->_scrollTop.getGlobalBounds().height * 1.5f + this->_startTrainingButton._sprite.getGlobalBounds().height + 40.0f + this->_planPanel.getGlobalBounds().height + 300.0f;
    float bottomBound = this->_scrollEdgeBot.getPosition().y - contentDeltaY;

    this->_scrollTopDestination.y += scrollDelta;
    if (this->_scrollTopDestination.y > topBound) { this->_scrollTopDestination.y = topBound; }
    else if (this->_scrollTopDestination.y < bottomBound) { this->_scrollTopDestination.y = bottomBound; }

    if (floorV2f(this->_scrollTop.getPosition()) != floorV2f(this->_scrollTopDestination))
    {
        ObjMoveTo(this->_scrollTop, this->_scrollTopDestination, 800.0f, dt);

        //  Start Training Button position
        this->_startTrainingButton._sprite.setPosition(
            this->_startTrainingButton._sprite.getPosition().x,
            this->_scrollTop.getPosition().y + this->_scrollTop.getGlobalBounds().height
        );

		//  Plan Panel position
        this->_planPanel.setPosition(
            this->_planPanel.getPosition().x,
            this->_startTrainingButton._sprite.getPosition().y + this->_startTrainingButton._sprite.getGlobalBounds().height + 40.0f
		);

		//  Plan Texts positions
        //  (To do)

		//  Scroll Bot position
        this->_scrollBot.setPosition(this->_scrollBot.getPosition().x, this->_planPanel.getPosition().y + this->_planPanel.getGlobalBounds().height + 300.0f);

        this->_data->input.resetScrollDelta();
    }
}
void HomeState::ScrollPanel_Draw(float dt)
{
    this->_data->window.draw(this->_scrollMid);
    this->_data->window.draw(this->_scrollTop);
    this->_data->window.draw(this->_scrollBot);
    this->_data->window.draw(this->_scrollEdgeTop);
    this->_data->window.draw(this->_scrollEdgeBot);
}

void HomeState::State_Init()
{
    this->_data->assets.loadTexture("Default Background", DEFAULT_BACKGROUND_FILEPATH);

    this->_background.setTexture(this->_data->assets.getTexture("Default Background"));

	ScrollPanel_Init();
	Buttons_Init();
	PlanPanel_Init();
    this->_scrollBot.setPosition(this->_scrollEdgeTop.getPosition().x, this->_planPanel.getPosition().y + this->_planPanel.getGlobalBounds().height + 300.0f);
}
void HomeState::State_HandleInput(sf::Event& event)
{
	Buttons_HandleInput(event);
	PlanPanel_HandleInput(event);
	ScrollPanel_HandleInput(event);
}
void HomeState::State_Update(float dt)
{
	Buttons_Update(dt);
	PlanPanel_Update(dt);
	ScrollPanel_Update(dt);
}
void HomeState::State_Draw(float dt)
{
    this->_data->window.draw(this->_background);

	PlanPanel_Draw(dt);
	Buttons_Draw(dt);
	ScrollPanel_Draw(dt);
}