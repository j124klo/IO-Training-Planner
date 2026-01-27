#include "DEFINITIONS.hpp"
#include "TrainingState.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

TrainingState::TrainingState(AppDataRef data) : GuiState(data)
{
    _currentMode = TrainingMode::DAY_SELECTION;
    _selectedDay = -1;
}

string TrainingState::getCurrentDate() {
    time_t t = time(0);   // Pobierz czas systemowy
    struct tm now;        // Struktura na datê (nie wskaŸnik)

    localtime_s(&now, &t);

    stringstream ss;
    ss << (now.tm_year + 1900) << '-'
        << setfill('0') << setw(2) << (now.tm_mon + 1) << '-'
        << setfill('0') << setw(2) << now.tm_mday;

    return ss.str();
}

// ==========================================
//              DAY SELECTION SCREEN
// ==========================================
void TrainingState::InitDaySelection()
{
    _dayButtonsShapes.clear();
    _dayLabels.clear();
    _dayButtonIndices.clear();

    _titleSelectDay.setFont(_font);
    _titleSelectDay.setString("SELECT WORKOUT DAY"); // EN
    _titleSelectDay.setCharacterSize(35);
    _titleSelectDay.setFillColor(sf::Color::White);

    sf::FloatRect b = _titleSelectDay.getLocalBounds();
    _titleSelectDay.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
    _titleSelectDay.setPosition(SCREAN_WIDTH / 2.0f + 100.0f, 80.0f);

    float startY = 180.0f;
    float startX = 450.0f;

    // 1. Generate buttons for planned days
    if (_currentPlan) {
        vector<int> days;
        for (const auto& entry : _currentPlan->entries) {
            bool exists = false;
            for (int d : days) if (d == entry.dayOfWeek) exists = true;
            if (!exists) days.push_back(entry.dayOfWeek);
        }
        std::sort(days.begin(), days.end());

        for (int day : days) {
            sf::RectangleShape btn;
            btn.setSize(sf::Vector2f(400.0f, 60.0f));
            btn.setFillColor(sf::Color::Transparent);
            btn.setOutlineColor(sf::Color::Cyan);
            btn.setOutlineThickness(2.0f);
            btn.setPosition(startX, startY);

            sf::Text label;
            label.setFont(_font);
            label.setString(getDayName(day));
            label.setCharacterSize(24);
            label.setFillColor(sf::Color::White);

            sf::FloatRect lb = label.getLocalBounds();
            label.setOrigin(lb.left + lb.width / 2.0f, lb.top + lb.height / 2.0f);
            label.setPosition(
                btn.getPosition().x + btn.getSize().x / 2.0f,
                btn.getPosition().y + btn.getSize().y / 2.0f - 5.0f
            );

            _dayButtonsShapes.push_back(btn);
            _dayLabels.push_back(label);
            _dayButtonIndices.push_back(day);

            startY += 80.0f;
        }
    }
    else {
        _titleSelectDay.setString("NO ACTIVE PLAN"); // EN
    }

    // 2. ADD "EMPTY WORKOUT" BUTTON (Always at the bottom)
    startY += 20.0f;

    _emptyWorkoutBtn.setSize(sf::Vector2f(400.0f, 60.0f));
    _emptyWorkoutBtn.setFillColor(sf::Color(50, 50, 50, 150));
    _emptyWorkoutBtn.setOutlineColor(sf::Color::White);
    _emptyWorkoutBtn.setOutlineThickness(2.0f);
    _emptyWorkoutBtn.setPosition(startX, startY);

    _emptyWorkoutLabel.setFont(_font);
    _emptyWorkoutLabel.setString("START EMPTY WORKOUT"); // EN
    _emptyWorkoutLabel.setCharacterSize(24);
    _emptyWorkoutLabel.setFillColor(sf::Color::White);

    sf::FloatRect eb = _emptyWorkoutLabel.getLocalBounds();
    _emptyWorkoutLabel.setOrigin(eb.left + eb.width / 2.0f, eb.top + eb.height / 2.0f);
    _emptyWorkoutLabel.setPosition(
        _emptyWorkoutBtn.getPosition().x + _emptyWorkoutBtn.getSize().x / 2.0f,
        _emptyWorkoutBtn.getPosition().y + _emptyWorkoutBtn.getSize().y / 2.0f - 5.0f
    );
}

// ==========================================
//              WORKOUT SESSION SCREEN
// ==========================================
void TrainingState::InitWorkoutSession(int dayOfWeek)
{
    _sessionExercises.clear();

    _titleWorkout.setFont(_font);
    _titleWorkout.setCharacterSize(40);
    _titleWorkout.setFillColor(sf::Color::Yellow);
    _titleWorkout.setPosition(350.0f, 40.0f);

    if (dayOfWeek == -1) {
        _titleWorkout.setString("EMPTY WORKOUT"); // EN
    }
    else {
        _titleWorkout.setString(getDayName(dayOfWeek));
    }

    _backButtonShape.setSize(sf::Vector2f(100.0f, 40.0f));
    _backButtonShape.setFillColor(sf::Color(100, 100, 100));
    _backButtonShape.setPosition(SCREAN_WIDTH - 150.0f, 40.0f);

    _backButtonLabel.setFont(_font);
    _backButtonLabel.setString("Back"); // EN
    _backButtonLabel.setCharacterSize(20);
    _backButtonLabel.setFillColor(sf::Color::White);
    _backButtonLabel.setPosition(SCREAN_WIDTH - 135.0f, 45.0f);

    _addExerciseBtn.setSize(sf::Vector2f(250.0f, 50.0f));
    _addExerciseBtn.setFillColor(sf::Color(0, 100, 200));
    _addExerciseLabel.setFont(_font);
    _addExerciseLabel.setString("+ ADD EXERCISE"); // EN
    _addExerciseLabel.setCharacterSize(20);
    _addExerciseLabel.setFillColor(sf::Color::White);

    // Load exercises from plan ONLY if a specific day is selected
    if (dayOfWeek != -1 && _currentPlan) {
        for (const auto& entry : _currentPlan->entries) {
            if (entry.dayOfWeek == dayOfWeek) {
                AddExerciseToSession(entry.exerciseId);
            }
        }
    }

    RecalculateLayout();
}
void TrainingState::InitExercisePicker()
{
    _allExercises.clear();
    try {
        _allExercises = this->_data->database.getAllExercises();
    }
    catch (exception& e) {}

    _titlePicker.setFont(_font);
    _titlePicker.setString("ADD EXERCISE"); // EN
    _titlePicker.setCharacterSize(35);
    _titlePicker.setFillColor(sf::Color::White);
    _titlePicker.setPosition(400.0f, 50.0f);

    _cancelPickerBtn.setSize(sf::Vector2f(100.0f, 40.0f));
    _cancelPickerBtn.setFillColor(sf::Color::Red);
    _cancelPickerBtn.setPosition(SCREAN_WIDTH - 150.0f, 50.0f);

    _cancelPickerLabel.setFont(_font);
    _cancelPickerLabel.setString("CANCEL"); // EN
    _cancelPickerLabel.setCharacterSize(18);
    _cancelPickerLabel.setFillColor(sf::Color::White);
    _cancelPickerLabel.setPosition(SCREAN_WIDTH - 140.0f, 58.0f);

    _pickerButtons.clear();
    _pickerLabels.clear();
    _pickerExerciseIds.clear();

    float py = 150.0f;
    float px = 400.0f;

    for (const auto& ex : _allExercises) {
        sf::RectangleShape btn;
        btn.setSize(sf::Vector2f(400.0f, 50.0f));
        btn.setFillColor(sf::Color(30, 30, 30));
        btn.setOutlineColor(sf::Color::White);
        btn.setOutlineThickness(1.0f);
        btn.setPosition(px, py);

        sf::Text txt;
        txt.setFont(_font);
        txt.setString(ex->name);
        txt.setCharacterSize(22);
        txt.setFillColor(sf::Color::White);
        txt.setPosition(px + 10.0f, py + 10.0f);

        _pickerButtons.push_back(btn);
        _pickerLabels.push_back(txt);
        _pickerExerciseIds.push_back(ex->id);

        py += 60.0f;
    }
}

void TrainingState::AddExerciseToSession(int exerciseId)
{
    auto exData = this->_data->database.getExerciseById(exerciseId);
    if (!exData) return;

    ActiveExercise activeEx;
    activeEx.exerciseId = exerciseId;
    activeEx.name = exData->name;

    // --- UNIT DETECTION ---
    string u1 = "kg";
    string u2 = "rep";

    bool hasDist = false;
    bool hasTime = false;
    for (auto vt : exData->valueTypes) {
        if (vt == ValueType::DISTANCE_KM) hasDist = true;
        if (vt == ValueType::TIME_SEC) hasTime = true;
    }

    if (hasDist) {
        u1 = "km";
        u2 = "min";
    }
    else if (hasTime) {
        u1 = "s";
        u2 = "";
    }

    activeEx.unit1 = u1;
    activeEx.unit2 = u2;

    activeEx.addSetBtn.setSize(sf::Vector2f(120.0f, 30.0f));
    activeEx.addSetBtn.setFillColor(sf::Color(50, 50, 50));
    activeEx.addSetBtn.setOutlineColor(sf::Color::White);
    activeEx.addSetBtn.setOutlineThickness(1.0f);

    activeEx.addSetLabel.setFont(_font);
    activeEx.addSetLabel.setString("+ Set"); // EN
    activeEx.addSetLabel.setCharacterSize(16);
    activeEx.addSetLabel.setFillColor(sf::Color::White);

    _sessionExercises.push_back(activeEx);
    AddSetToExercise(_sessionExercises.back());
}
void TrainingState::AddSetToExercise(ActiveExercise& ex)
{
    SetRow row;
    row.setNumber = ex.sets.size() + 1;
    row.isSaved = false;

    // Input 1
    row.input1.shape.setSize(sf::Vector2f(80.0f, 35.0f));
    row.input1.shape.setFillColor(sf::Color(20, 20, 20));
    row.input1.shape.setOutlineColor(sf::Color(100, 100, 100));
    row.input1.shape.setOutlineThickness(1.0f);
    row.input1.text.setFont(_font);
    row.input1.text.setString(ex.unit1);
    row.input1.text.setCharacterSize(20);
    row.input1.text.setFillColor(sf::Color(150, 150, 150));

    row.input1.valueString = "";
    row.input1.placeholder = ex.unit1;
    row.input1.isVisible = true;

    // Input 2
    row.input2.shape.setSize(sf::Vector2f(80.0f, 35.0f));
    row.input2.shape.setFillColor(sf::Color(20, 20, 20));
    row.input2.shape.setOutlineColor(sf::Color(100, 100, 100));
    row.input2.shape.setOutlineThickness(1.0f);
    row.input2.text.setFont(_font);
    row.input2.text.setString(ex.unit2);
    row.input2.text.setCharacterSize(20);
    row.input2.text.setFillColor(sf::Color(150, 150, 150));

    row.input2.valueString = "";
    row.input2.placeholder = ex.unit2;

    if (ex.unit2.empty()) {
        row.input2.isVisible = false;
    }
    else {
        row.input2.isVisible = true;
    }

    // Save Button
    row.saveButton.setSize(sf::Vector2f(80.0f, 35.0f));
    row.saveButton.setFillColor(sf::Color(0, 100, 0));
    row.saveLabel.setFont(_font);
    row.saveLabel.setString("OK"); // EN
    row.saveLabel.setCharacterSize(18);
    row.saveLabel.setFillColor(sf::Color::White);

    ex.sets.push_back(row);
    RecalculateLayout();
}
void TrainingState::RecalculateLayout()
{
    float currentY = 120.0f;
    float xBtn = 750.0f;

    for (auto& ex : _sessionExercises) {
        ex.headerY = currentY;
        currentY += 40.0f;

        for (auto& row : ex.sets) {
            float x1 = 550.0f;
            float x2 = 650.0f;

            row.input1.shape.setPosition(x1, currentY);
            row.input1.text.setPosition(x1 + 5, currentY + 5);

            if (row.input2.isVisible) {
                row.input2.shape.setPosition(x2, currentY);
                row.input2.text.setPosition(x2 + 5, currentY + 5);
            }

            row.saveButton.setPosition(xBtn, currentY);

            sf::FloatRect b = row.saveLabel.getLocalBounds();
            row.saveLabel.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
            row.saveLabel.setPosition(xBtn + 40.0f, currentY + 17.0f);

            currentY += 45.0f;
        }

        ex.addSetBtn.setPosition(xBtn, currentY);
        ex.addSetLabel.setPosition(xBtn + 10.0f, currentY + 5.0f);
        currentY += 60.0f;
    }

    _addExerciseBtn.setPosition(SCREAN_WIDTH / 2.0f - 125.0f, currentY + 20.0f);
    sf::FloatRect eb = _addExerciseLabel.getLocalBounds();
    _addExerciseLabel.setOrigin(eb.left + eb.width / 2.0f, eb.top + eb.height / 2.0f);
    _addExerciseLabel.setPosition(
        _addExerciseBtn.getPosition().x + 125.0f,
        _addExerciseBtn.getPosition().y + 25.0f
    );
}

void TrainingState::handleInputBoxClick(InputBox& box, sf::Vector2i mousePos)
{
    if (!box.isVisible) return;

    if (box.shape.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
        box.isSelected = true;
        box.shape.setOutlineColor(sf::Color::Yellow);
        box.shape.setOutlineThickness(2.0f);
        if (box.valueString.empty()) box.text.setString("");
    }
    else {
        box.isSelected = false;
        box.shape.setOutlineColor(sf::Color(100, 100, 100));
        box.shape.setOutlineThickness(1.0f);
        if (box.valueString.empty()) {
            box.text.setString(box.placeholder);
            box.text.setFillColor(sf::Color(150, 150, 150));
        }
    }
}
void TrainingState::handleTyping(InputBox& box, sf::Uint32 unicode)
{
    if (!box.isSelected || !box.isVisible) return;

    if (unicode == 8) {
        if (!box.valueString.empty()) box.valueString.pop_back();
    }
    else if ((unicode >= 48 && unicode <= 57) || unicode == 46) {
        if (box.valueString.size() < 6) box.valueString += static_cast<char>(unicode);
    }

    box.text.setString(box.valueString);
    box.text.setFillColor(sf::Color::White);
}

string TrainingState::getDayName(int day) {
    switch (day) {
    case 1: return "MONDAY";
    case 2: return "TUESDAY";
    case 3: return "WEDNESDAY";
    case 4: return "THURSDAY";
    case 5: return "FRIDAY";
    case 6: return "SATURDAY";
    case 7: return "SUNDAY";
    default: return "DAY " + to_string(day);
    }
}

void TrainingState::State_Init()
{
    this->_background.setTexture(this->_data->assets.getTexture("Default Background"));
    _font = this->_data->assets.getFont("Default Font");

    if (this->_data->currentUser && this->_data->currentUser->currentPlanId.has_value()) {
        int planId = this->_data->currentUser->currentPlanId.value();
        _currentPlan = this->_data->database.getTrainingPlanById(planId);
    }

    InitDaySelection();
}
void TrainingState::State_HandleInput(sf::Event& event)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(this->_data->window);

    if (event.type == sf::Event::TextEntered && _currentMode == TrainingMode::WORKOUT_SESSION) {
        for (auto& ex : _sessionExercises) {
            for (auto& row : ex.sets) {
                if (!row.isSaved) {
                    handleTyping(row.input1, event.text.unicode);
                    handleTyping(row.input2, event.text.unicode);
                }
            }
        }
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {

        if (_currentMode == TrainingMode::DAY_SELECTION) {
            // Select Day from Plan
            for (size_t i = 0; i < _dayButtonsShapes.size(); i++) {
                if (_dayButtonsShapes[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    _selectedDay = _dayButtonIndices[i];
                    _currentMode = TrainingMode::WORKOUT_SESSION;
                    InitWorkoutSession(_selectedDay);
                    return;
                }
            }
            // Select Empty Workout
            if (_emptyWorkoutBtn.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                _selectedDay = -1; // -1 means empty workout
                _currentMode = TrainingMode::WORKOUT_SESSION;
                InitWorkoutSession(-1);
                return;
            }
        }
        else if (_currentMode == TrainingMode::WORKOUT_SESSION) {
            if (_backButtonShape.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                _currentMode = TrainingMode::DAY_SELECTION;
                InitDaySelection();
                return;
            }

            if (_addExerciseBtn.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                _currentMode = TrainingMode::EXERCISE_PICKER;
                InitExercisePicker();
                return;
            }

            for (auto& ex : _sessionExercises) {
                if (ex.addSetBtn.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    AddSetToExercise(ex);
                    return;
                }

                for (auto& row : ex.sets) {
                    if (row.isSaved) continue;

                    handleInputBoxClick(row.input1, mousePos);
                    handleInputBoxClick(row.input2, mousePos);

                    if (row.saveButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                        string val1 = row.input1.valueString;
                        string val2 = row.input2.valueString;

                        if (row.input2.isVisible && val2.empty()) continue;
                        if (val1.empty()) continue;

                        float v1 = stof(val1);
                        int v2 = (val2.empty()) ? 0 : stoi(val2);

                        this->_data->database.addWorkoutLog(
                            this->_data->currentUser->id,
                            ex.exerciseId,
                            getCurrentDate(),
                            row.setNumber,
                            v1,
                            v2
                        );

                        row.isSaved = true;
                        row.saveButton.setFillColor(sf::Color(100, 100, 100));
                        row.saveLabel.setString("V");
                        row.input1.shape.setOutlineColor(sf::Color::Green);
                        row.input2.shape.setOutlineColor(sf::Color::Green);
                    }
                }
            }
        }
        else if (_currentMode == TrainingMode::EXERCISE_PICKER) {
            if (_cancelPickerBtn.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                _currentMode = TrainingMode::WORKOUT_SESSION;
                return;
            }

            for (size_t i = 0; i < _pickerButtons.size(); i++) {
                if (_pickerButtons[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    int id = _pickerExerciseIds[i];
                    AddExerciseToSession(id);
                    _currentMode = TrainingMode::WORKOUT_SESSION;
                    return;
                }
            }
        }
    }
}
void TrainingState::State_Update(float dt) {}
void TrainingState::State_Draw(float dt)
{
    this->_data->window.draw(this->_background);

    if (_currentMode == TrainingMode::DAY_SELECTION) {
        this->_data->window.draw(_titleSelectDay);
        for (const auto& s : _dayButtonsShapes) this->_data->window.draw(s);
        for (const auto& l : _dayLabels) this->_data->window.draw(l);

        // Draw Empty Workout Button
        this->_data->window.draw(_emptyWorkoutBtn);
        this->_data->window.draw(_emptyWorkoutLabel);
    }
    else if (_currentMode == TrainingMode::WORKOUT_SESSION) {
        this->_data->window.draw(_titleWorkout);
        this->_data->window.draw(_backButtonShape);
        this->_data->window.draw(_backButtonLabel);

        for (const auto& ex : _sessionExercises) {
            sf::Text title;
            title.setFont(_font);
            title.setString(ex.name);
            title.setCharacterSize(24);
            title.setFillColor(sf::Color::White);
            title.setPosition(250.0f, ex.headerY);
            title.setStyle(sf::Text::Bold);
            this->_data->window.draw(title);

            for (const auto& row : ex.sets) {
                sf::Text setNo;
                setNo.setFont(_font);
                setNo.setString(to_string(row.setNumber) + ".");
                setNo.setCharacterSize(20);
                setNo.setFillColor(sf::Color(180, 180, 180));
                setNo.setPosition(220.0f, row.input1.shape.getPosition().y + 5.0f);
                this->_data->window.draw(setNo);

                this->_data->window.draw(row.input1.shape);
                this->_data->window.draw(row.input1.text);

                if (row.input2.isVisible) {
                    this->_data->window.draw(row.input2.shape);
                    this->_data->window.draw(row.input2.text);
                }

                this->_data->window.draw(row.saveButton);
                this->_data->window.draw(row.saveLabel);
            }

            this->_data->window.draw(ex.addSetBtn);
            this->_data->window.draw(ex.addSetLabel);
        }

        this->_data->window.draw(_addExerciseBtn);
        this->_data->window.draw(_addExerciseLabel);
    }
    else if (_currentMode == TrainingMode::EXERCISE_PICKER) {
        this->_data->window.draw(_titlePicker);
        this->_data->window.draw(_cancelPickerBtn);
        this->_data->window.draw(_cancelPickerLabel);

        for (const auto& btn : _pickerButtons) this->_data->window.draw(btn);
        for (const auto& lbl : _pickerLabels) this->_data->window.draw(lbl);
    }
}