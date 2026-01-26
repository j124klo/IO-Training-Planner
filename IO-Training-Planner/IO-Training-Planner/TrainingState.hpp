#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "Models.hpp"

using namespace std;

enum class TrainingMode { DAY_SELECTION, WORKOUT_SESSION, EXERCISE_PICKER };

struct InputBox {
    sf::RectangleShape shape;
    sf::Text text;
    string valueString;
    string placeholder; // e.g. "kg", "km", "min"
    bool isSelected = false;
    bool isVisible = true;
};

struct SetRow {
    int setNumber;
    InputBox input1;
    InputBox input2;
    sf::RectangleShape saveButton;
    sf::Text saveLabel;
    bool isSaved = false;
};

struct ActiveExercise {
    int exerciseId;
    string name;

    // Units for this exercise
    string unit1;
    string unit2;

    vector<SetRow> sets;

    sf::RectangleShape addSetBtn;
    sf::Text addSetLabel;

    float headerY;
};

class TrainingState : public GuiState
{
private:
    sf::Sprite _background;
    sf::Font _font;

    TrainingMode _currentMode;
    shared_ptr<TrainingPlan> _currentPlan;
    int _selectedDay;

    // --- DAY SELECTION SCREEN ---
    sf::Text _titleSelectDay;
    vector<sf::RectangleShape> _dayButtonsShapes;
    vector<sf::Text> _dayLabels;
    vector<int> _dayButtonIndices;

    // NEW: Empty Workout Button
    sf::RectangleShape _emptyWorkoutBtn;
    sf::Text _emptyWorkoutLabel;

    // --- WORKOUT SESSION SCREEN ---
    sf::Text _titleWorkout;
    sf::RectangleShape _backButtonShape;
    sf::Text _backButtonLabel;

    vector<ActiveExercise> _sessionExercises;

    sf::RectangleShape _addExerciseBtn;
    sf::Text _addExerciseLabel;

    // --- EXERCISE PICKER SCREEN ---
    sf::Text _titlePicker;
    sf::RectangleShape _cancelPickerBtn;
    sf::Text _cancelPickerLabel;

    vector<shared_ptr<Exercise>> _allExercises;
    vector<sf::RectangleShape> _pickerButtons;
    vector<sf::Text> _pickerLabels;
    vector<int> _pickerExerciseIds;

    // --- METHODS ---
    void InitDaySelection();
    void InitWorkoutSession(int dayOfWeek);
    void InitExercisePicker();

    void RecalculateLayout();
    void AddSetToExercise(ActiveExercise& ex);
    void AddExerciseToSession(int exerciseId);

    void handleInputBoxClick(InputBox& box, sf::Vector2i mousePos);
    void handleTyping(InputBox& box, sf::Uint32 unicode);

    string getDayName(int day);

    string getCurrentDate();

public:
    TrainingState(AppDataRef data);

    void State_Init();
    void State_HandleInput(sf::Event& event);
    void State_Update(float dt);
    void State_Draw(float dt);
};