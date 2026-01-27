#include "DEFINITIONS.hpp"
#include "ProgressHistoryState.hpp"
#include "ProgressState.hpp"
#include <iomanip>

using namespace std;

ProgressHistoryState::ProgressHistoryState(AppDataRef data) : GuiState(data)
{

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

void ProgressHistoryState::Buttons_Init()
{
    this->_data->assets.loadTexture("Back Button", BACK_BUTTON_FILEPATH);

	this->_backButton._sprite.setTexture(this->_data->assets.getTexture("Back Button"));

    float menuButtonWidth;
    float menuButtonHeight;
    sf::Vector2f menuButtonPosition = GetMenuButtonPosition(menuButtonWidth, menuButtonHeight);

	this->_backButton._accessible = true;
    this->_backButton._pressed = false;
    this->_backButton._update = false;
    this->_backButton._rect = sf::IntRect(0, 0, 300, 300);
    this->_backButton._sprite.setTextureRect(this->_backButton._rect);
    this->_backButton._sprite.setScale(0.35f, 0.35f);
    this->_backButton._sprite.setPosition( menuButtonPosition.x, SCREAN_HEIGHT - this->_backButton._sprite.getGlobalBounds().height - 40.0f );
}
void ProgressHistoryState::Buttons_HandleInput(sf::Event& event)
{
    if (this->_data->input.isSpriteClickedAndReleased(this->_backButton._sprite, sf::Mouse::Left, this->_data->window, this->_backButton._pressed, this->_backButton._update) and this->_backButton._accessible)
    {
        this->_data->machine.AddState(StateRef(new ProgressState(this->_data)), true);
    }
}
void ProgressHistoryState::Buttons_Update(float dt)
{
    if (IsGuiBarVisible())
    {
		this->_backButton._accessible = false;
    }
    else
    {
		this->_backButton._accessible = true;
    }

    if (this->_backButton._update)
    {
        this->_backButton._update = false;
        this->_backButton._rect.left = this->_backButton._pressed ? 300 : 0;
        this->_backButton._sprite.setTextureRect(this->_backButton._rect);
    }
}
void ProgressHistoryState::Buttons_Draw(float dt)
{
	this->_data->window.draw(this->_backButton._sprite);
}

void ProgressHistoryState::Logs_Init()
{
    this->_title.setFont(this->_data->assets.getFont("Default Font"));

	this->_logs.clear();
    if (this->_data->currentUser) {
        this->_logs = this->_data->database.getWorkoutLogs(this->_data->currentUser->id);
    }

    auto addText = [&](vector<sf::Text>& v, string s, float x, float y, sf::Color c, int size = 30, bool bold = false, bool center = false) {
        sf::Text t;
        t.setFont(this->_data->assets.getFont("Default Font"));
        t.setString(s);
        t.setCharacterSize(size);
        t.setFillColor(c);
        if (bold)   t.setStyle(sf::Text::Bold);
        if (center) t.setPosition(x - t.getGlobalBounds().width / 2, y);
        else        t.setPosition(v[0].getPosition().x, y);
        v.push_back(t);
    };

    this->_title.setString("TRAINING HISTORY");
    this->_title.setCharacterSize(50);
    this->_title.setFillColor(sf::Color::White);
    this->_title.setPosition(
        this->_scrollTop.getPosition().x + this->_scrollTop.getGlobalBounds().width / 2 - this->_title.getGlobalBounds().width / 2,
        this->_scrollTop.getPosition().y + this->_scrollTop.getGlobalBounds().height
    );

    float currentY = this->_title.getPosition().y + this->_title.getGlobalBounds().height + 40.0f;

    float colDate = this->_scrollEdgeTop.getPosition().x + this->_scrollEdgeTop.getGlobalBounds().width * 3 / 12;
    float colName = this->_scrollEdgeTop.getPosition().x + this->_scrollEdgeTop.getGlobalBounds().width * 5 / 12;
    float colSet = this->_scrollEdgeTop.getPosition().x + this->_scrollEdgeTop.getGlobalBounds().width * 7 / 12;
    float colRes = this->_scrollEdgeTop.getPosition().x + this->_scrollEdgeTop.getGlobalBounds().width * 8 / 12;

    addText(this->_logDates, "Date", colDate, currentY, sf::Color::Cyan, 35, true, true);
    addText(this->_logExercises,"Exercise", colName, currentY, sf::Color::Cyan, 35, true, true);
    addText(this->_logSets,"Set", colSet, currentY, sf::Color::Cyan, 35, true, true);
    addText(this->_logResults,"Result", colRes, currentY, sf::Color::Cyan, 35, true, true);

    currentY += this->_logExercises[0].getGlobalBounds().height + 40.0f;

    for (size_t i = 0; i < this->_logs.size(); i++)
    {
        addText(this->_logDates, this->_logs[i].date, colDate, currentY, sf::Color(200, 200, 200));
        addText(this->_logExercises, this->_logs[i].exerciseNameVal, colName, currentY, sf::Color::White);
        addText(this->_logSets, to_string(this->_logs[i].setNumber), colSet, currentY, sf::Color(200, 200, 200), 30, false, true);
        addText(this->_logResults, formatLogResult(this->_logs[i]), colRes, currentY, sf::Color::White);

        currentY += this->_logExercises[i+1].getGlobalBounds().height + 40.0f;
    }

    if (this->_logExercises.size() == 1)
    {
		addText(this->_logDates, "- - -", colDate, currentY, sf::Color(200, 200, 200), 30, false, true);
		addText(this->_logExercises, "- - -", colName, currentY, sf::Color(200, 200, 200), 30, false, true);
		addText(this->_logSets, "- - -", colSet, currentY, sf::Color(200, 200, 200), 30, false, true);
		addText(this->_logResults, "- - -", colRes, currentY, sf::Color(200, 200, 200), 30, false, true);

        currentY += this->_logExercises[1].getGlobalBounds().height + 40.0f;
    }

    this->_scrollBot.setPosition(this->_scrollEdgeTop.getPosition().x, currentY - 40.f);
}
void ProgressHistoryState::Logs_HandleInput(sf::Event& event)
{

}
void ProgressHistoryState::Logs_Update(float dt)
{

}
void ProgressHistoryState::Logs_Draw(float dt)
{
    this->_data->window.draw(this->_title);
    for (const auto& dates : this->_logDates) {
        this->_data->window.draw(dates);
    }
    for (const auto& exercises : this->_logExercises) {
        this->_data->window.draw(exercises);
    }
    for (const auto& sets : this->_logSets) {
        this->_data->window.draw(sets);
    }
    for (const auto& results : this->_logResults) {
        this->_data->window.draw(results);
    }
}

void ProgressHistoryState::ScrollPanel_Init()
{
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
void ProgressHistoryState::ScrollPanel_HandleInput(sf::Event& event)
{
    this->_data->input.handleScrollEvent(event);
}
void ProgressHistoryState::ScrollPanel_Update(float dt)
{
    float scrollDelta = this->_data->input.getScrollDelta() * 80.0f;

    float topBound = this->_scrollEdgeTop.getGlobalBounds().height - this->_scrollTop.getGlobalBounds().height / 2.0f;

    float contentDeltaY = this->_scrollTop.getGlobalBounds().height * 1.5f + this->_title.getGlobalBounds().height + 40.0f;
    for (size_t i = 0; i < this->_logExercises.size(); i++)
    {
        contentDeltaY += this->_logExercises[i].getGlobalBounds().height + 40.0f;
    }
    float bottomBound = this->_scrollEdgeBot.getPosition().y - contentDeltaY + 40.0f;

    if (bottomBound >= topBound)
    {
        this->_scrollTopDestination.y = topBound;
    }
    else
    {
        this->_scrollTopDestination.y += scrollDelta;
        if (this->_scrollTopDestination.y > topBound) { this->_scrollTopDestination.y = topBound; }
        else if (this->_scrollTopDestination.y < bottomBound) { this->_scrollTopDestination.y = bottomBound; }
    }

    if (floorV2f(this->_scrollTop.getPosition()) != floorV2f(this->_scrollTopDestination))
    {
        ObjMoveTo(this->_scrollTop, this->_scrollTopDestination, 800.0f, dt);

        this->_title.setPosition(this->_title.getPosition().x, this->_scrollTop.getPosition().y + this->_scrollTop.getGlobalBounds().height);

        float currentY = this->_title.getPosition().y + this->_title.getGlobalBounds().height + 40.0f;
        for (size_t i = 0; i < this->_logExercises.size(); i++)
        {
            this->_logDates[i].setPosition(this->_logDates[i].getPosition().x, currentY);
            this->_logExercises[i].setPosition(this->_logExercises[i].getPosition().x, currentY);
            this->_logSets[i].setPosition(this->_logSets[i].getPosition().x, currentY);
            this->_logResults[i].setPosition(this->_logResults[i].getPosition().x, currentY);
            currentY += this->_logExercises[i].getGlobalBounds().height + 40.0f;

            this->_scrollBot.setPosition(this->_scrollBot.getPosition().x, currentY - 40.f);
        }

        this->_data->input.resetScrollDelta();
    }
}
void ProgressHistoryState::ScrollPanel_Draw(float dt)
{
    this->_data->window.draw(this->_scrollMid);
    this->_data->window.draw(this->_scrollTop);
    this->_data->window.draw(this->_scrollBot);
    this->_data->window.draw(this->_scrollEdgeTop);
    this->_data->window.draw(this->_scrollEdgeBot);
}

void ProgressHistoryState::State_Init()
{
    this->_background.setTexture(this->_data->assets.getTexture("Default Background"));

	Buttons_Init();

	ScrollPanel_Init();
	Logs_Init();

}
void ProgressHistoryState::State_HandleInput(sf::Event& event)
{
	Buttons_HandleInput(event);
    Logs_HandleInput(event);
	ScrollPanel_HandleInput(event);
}
void ProgressHistoryState::State_Update(float dt)
{
	Buttons_Update(dt);
	Logs_Update(dt);
	ScrollPanel_Update(dt);
}
void ProgressHistoryState::State_Draw(float dt)
{
    this->_data->window.draw(this->_background);

	Logs_Draw(dt);
	ScrollPanel_Draw(dt);
    
	Buttons_Draw(dt);
}