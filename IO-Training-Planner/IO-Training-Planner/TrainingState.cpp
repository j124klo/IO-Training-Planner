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

// ==========================================
//              EKRAN WYBORU DNIA
// ==========================================
void TrainingState::InitDaySelection()
{
    _dayButtonsShapes.clear();
    _dayLabels.clear();
    _dayButtonIndices.clear();

    _titleSelectDay.setFont(_font);
    _titleSelectDay.setString("WYBIERZ DZIEN TRENINGOWY");
    _titleSelectDay.setCharacterSize(35);
    _titleSelectDay.setFillColor(sf::Color::White);

    sf::FloatRect b = _titleSelectDay.getLocalBounds();
    _titleSelectDay.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
    _titleSelectDay.setPosition(SCREAN_WIDTH / 2.0f + 100.0f, 80.0f);

    if (!_currentPlan) {
        _titleSelectDay.setString("BRAK AKTYWNEGO PLANU");
        return;
    }

    vector<int> days;
    for (const auto& entry : _currentPlan->entries) {
        bool exists = false;
        for (int d : days) if (d == entry.dayOfWeek) exists = true;
        if (!exists) days.push_back(entry.dayOfWeek);
    }
    std::sort(days.begin(), days.end());

    float startY = 180.0f;
    float startX = 450.0f;

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

// ==========================================
//              EKRAN TRENINGU
// ==========================================
void TrainingState::InitWorkoutSession(int dayOfWeek)
{
    _sessionExercises.clear();

    // Tytu³
    _titleWorkout.setFont(_font);
    _titleWorkout.setString(getDayName(dayOfWeek));
    _titleWorkout.setCharacterSize(40);
    _titleWorkout.setFillColor(sf::Color::Yellow);
    _titleWorkout.setPosition(350.0f, 40.0f);

    // Przycisk Wstecz
    _backButtonShape.setSize(sf::Vector2f(100.0f, 40.0f));
    _backButtonShape.setFillColor(sf::Color(100, 100, 100));
    _backButtonShape.setPosition(SCREAN_WIDTH - 150.0f, 40.0f);

    _backButtonLabel.setFont(_font);
    _backButtonLabel.setString("Wroc");
    _backButtonLabel.setCharacterSize(20);
    _backButtonLabel.setFillColor(sf::Color::White);
    _backButtonLabel.setPosition(SCREAN_WIDTH - 135.0f, 45.0f);

    // Przycisk Dodaj Æwiczenie
    _addExerciseBtn.setSize(sf::Vector2f(250.0f, 50.0f));
    _addExerciseBtn.setFillColor(sf::Color(0, 100, 200));
    _addExerciseLabel.setFont(_font);
    _addExerciseLabel.setString("+ DODAJ CWICZENIE");
    _addExerciseLabel.setCharacterSize(20);
    _addExerciseLabel.setFillColor(sf::Color::White);

    // 1. Dodaj æwiczenia z planu
    if (_currentPlan) {
        for (const auto& entry : _currentPlan->entries) {
            if (entry.dayOfWeek == dayOfWeek) {
                AddExerciseToSession(entry.exerciseId);
            }
        }
    }

    // 2. Przelicz uk³ad
    RecalculateLayout();
}

void TrainingState::AddExerciseToSession(int exerciseId)
{
    auto exData = this->_data->database.getExerciseById(exerciseId);
    if (!exData) return;

    ActiveExercise activeEx;
    activeEx.exerciseId = exerciseId;
    activeEx.name = exData->name;

    // Przycisk "Dodaj Seriê"
    activeEx.addSetBtn.setSize(sf::Vector2f(120.0f, 30.0f));
    activeEx.addSetBtn.setFillColor(sf::Color(50, 50, 50));
    activeEx.addSetBtn.setOutlineColor(sf::Color::White);
    activeEx.addSetBtn.setOutlineThickness(1.0f);

    activeEx.addSetLabel.setFont(_font);
    activeEx.addSetLabel.setString("+ Seria");
    activeEx.addSetLabel.setCharacterSize(16);
    activeEx.addSetLabel.setFillColor(sf::Color::White);

    _sessionExercises.push_back(activeEx);

    // Dodaj jedn¹ pust¹ seriê na start
    AddSetToExercise(_sessionExercises.back());
}

void TrainingState::AddSetToExercise(ActiveExercise& ex)
{
    SetRow row;
    row.setNumber = ex.sets.size() + 1;
    row.isSaved = false;

    // Input Waga
    row.weightInput.shape.setSize(sf::Vector2f(80.0f, 35.0f));
    row.weightInput.shape.setFillColor(sf::Color(20, 20, 20));
    row.weightInput.shape.setOutlineColor(sf::Color(100, 100, 100));
    row.weightInput.shape.setOutlineThickness(1.0f);
    row.weightInput.text.setFont(_font);
    row.weightInput.text.setString("kg");
    row.weightInput.text.setCharacterSize(20);
    row.weightInput.text.setFillColor(sf::Color(150, 150, 150));

    row.weightInput.valueString = "";
    row.weightInput.placeholder = "kg";

    // Input Reps
    row.repsInput.shape.setSize(sf::Vector2f(80.0f, 35.0f));
    row.repsInput.shape.setFillColor(sf::Color(20, 20, 20));
    row.repsInput.shape.setOutlineColor(sf::Color(100, 100, 100));
    row.repsInput.shape.setOutlineThickness(1.0f);
    row.repsInput.text.setFont(_font);
    row.repsInput.text.setString("rep");
    row.repsInput.text.setCharacterSize(20);
    row.repsInput.text.setFillColor(sf::Color(150, 150, 150));

    row.repsInput.valueString = "";
    row.repsInput.placeholder = "rep";

    // Przycisk Zapisz
    row.saveButton.setSize(sf::Vector2f(80.0f, 35.0f));
    row.saveButton.setFillColor(sf::Color(0, 100, 0)); // Zielony
    row.saveLabel.setFont(_font);
    row.saveLabel.setString("OK");
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
            float xWeight = 550.0f;
            float xReps = 650.0f;

            row.weightInput.shape.setPosition(xWeight, currentY);
            row.weightInput.text.setPosition(xWeight + 5, currentY + 5);

            row.repsInput.shape.setPosition(xReps, currentY);
            row.repsInput.text.setPosition(xReps + 5, currentY + 5);

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

// ==========================================
//              PICKER (WYBÓR ÆWICZENIA)
// ==========================================
void TrainingState::InitExercisePicker()
{
    _allExercises.clear();

    try {
        _allExercises = this->_data->database.getAllExercises();
    }
    catch (exception& e) {
        cout << "Error fetching exercises: " << e.what() << endl;
    }

    _titlePicker.setFont(_font);
    _titlePicker.setString("DODAJ CWICZENIE");
    _titlePicker.setCharacterSize(35);
    _titlePicker.setFillColor(sf::Color::White);
    _titlePicker.setPosition(400.0f, 50.0f);

    _cancelPickerBtn.setSize(sf::Vector2f(100.0f, 40.0f));
    _cancelPickerBtn.setFillColor(sf::Color::Red);
    _cancelPickerBtn.setPosition(SCREAN_WIDTH - 150.0f, 50.0f);

    _cancelPickerLabel.setFont(_font);
    _cancelPickerLabel.setString("ANULUJ");
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
        // ZMIANA TUTAJ: u¿ywamy strza³ki -> bo ex to wskaŸnik
        txt.setString(ex->name);
        txt.setCharacterSize(22);
        txt.setFillColor(sf::Color::White);
        txt.setPosition(px + 10.0f, py + 10.0f);

        _pickerButtons.push_back(btn);
        _pickerLabels.push_back(txt);

        // ZMIANA TUTAJ: u¿ywamy strza³ki -> bo ex to wskaŸnik
        _pickerExerciseIds.push_back(ex->id);

        py += 60.0f;
    }
}


// --- INPUT HELPERS ---
void TrainingState::handleInputBoxClick(InputBox& box, sf::Vector2i mousePos)
{
    if (box.shape.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
        box.isSelected = true;
        box.shape.setOutlineColor(sf::Color::Yellow);
        box.shape.setOutlineThickness(2.0f);

        if (box.valueString.empty()) {
            box.text.setString("");
        }
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
    if (!box.isSelected) return;

    if (unicode == 8) { // Backspace
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
    case 1: return "PONIEDZIALEK";
    case 2: return "WTOREK";
    case 3: return "SRODA";
    case 4: return "CZWARTEK";
    case 5: return "PIATEK";
    case 6: return "SOBOTA";
    case 7: return "NIEDZIELA";
    default: return "DZIEN " + to_string(day);
    }
}

// ==========================================
//              HANDLE INPUT
// ==========================================
void TrainingState::State_HandleInput(sf::Event& event)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(this->_data->window);

    // 1. PISANIE
    if (event.type == sf::Event::TextEntered && _currentMode == TrainingMode::WORKOUT_SESSION) {
        for (auto& ex : _sessionExercises) {
            for (auto& row : ex.sets) {
                if (!row.isSaved) {
                    handleTyping(row.weightInput, event.text.unicode);
                    handleTyping(row.repsInput, event.text.unicode);
                }
            }
        }
    }

    // 2. KLIKNIÊCIA
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {

        if (_currentMode == TrainingMode::DAY_SELECTION) {
            for (size_t i = 0; i < _dayButtonsShapes.size(); i++) {
                if (_dayButtonsShapes[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    _selectedDay = _dayButtonIndices[i];
                    _currentMode = TrainingMode::WORKOUT_SESSION;
                    InitWorkoutSession(_selectedDay);
                    return;
                }
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

                    handleInputBoxClick(row.weightInput, mousePos);
                    handleInputBoxClick(row.repsInput, mousePos);

                    if (row.saveButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                        string wStr = row.weightInput.valueString;
                        string rStr = row.repsInput.valueString;

                        if (wStr.empty() || rStr.empty()) continue;

                        float weight = stof(wStr);
                        int reps = stoi(rStr);

                        string today = "2026-01-25";
                        this->_data->database.addWorkoutLog(
                            this->_data->currentUser->id,
                            ex.exerciseId,
                            today,
                            row.setNumber,
                            weight,
                            reps
                        );

                        row.isSaved = true;
                        row.saveButton.setFillColor(sf::Color(100, 100, 100));
                        row.saveLabel.setString("V");
                        row.weightInput.shape.setOutlineColor(sf::Color::Green);
                        row.repsInput.shape.setOutlineColor(sf::Color::Green);
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
                setNo.setPosition(220.0f, row.weightInput.shape.getPosition().y + 5.0f);
                this->_data->window.draw(setNo);

                this->_data->window.draw(row.weightInput.shape);
                this->_data->window.draw(row.weightInput.text);
                this->_data->window.draw(row.repsInput.shape);
                this->_data->window.draw(row.repsInput.text);

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