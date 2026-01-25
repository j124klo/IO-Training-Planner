#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <memory> // Wa¿ne dla shared_ptr
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "Models.hpp"

using namespace std;

enum class TrainingMode { DAY_SELECTION, WORKOUT_SESSION, EXERCISE_PICKER };

// Struktura pola tekstowego
struct InputBox {
    sf::RectangleShape shape;
    sf::Text text;
    string valueString;
    string placeholder;
    bool isSelected = false;
};

// Struktura reprezentuj¹ca JEDN¥ SERIÊ
struct SetRow {
    int setNumber;
    InputBox weightInput;
    InputBox repsInput;
    sf::RectangleShape saveButton;
    sf::Text saveLabel;
    bool isSaved = false;
};

// Struktura reprezentuj¹ca CA£E ÆWICZENIE w sesji
struct ActiveExercise {
    int exerciseId;
    string name;

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

    // --- EKRAN WYBORU DNIA ---
    sf::Text _titleSelectDay;
    vector<sf::RectangleShape> _dayButtonsShapes;
    vector<sf::Text> _dayLabels;
    vector<int> _dayButtonIndices;

    // --- EKRAN TRENINGU ---
    sf::Text _titleWorkout;
    sf::RectangleShape _backButtonShape;
    sf::Text _backButtonLabel;

    vector<ActiveExercise> _sessionExercises;

    sf::RectangleShape _addExerciseBtn;
    sf::Text _addExerciseLabel;

    // --- EKRAN WYBORU ÆWICZENIA (PICKER) ---
    sf::Text _titlePicker;
    sf::RectangleShape _cancelPickerBtn;
    sf::Text _cancelPickerLabel;

    // ZMIANA TUTAJ: vector<shared_ptr<Exercise>> zamiast vector<Exercise>
    vector<shared_ptr<Exercise>> _allExercises;

    vector<sf::RectangleShape> _pickerButtons;
    vector<sf::Text> _pickerLabels;
    vector<int> _pickerExerciseIds;

    // --- METODY ---
    void InitDaySelection();
    void InitWorkoutSession(int dayOfWeek);
    void InitExercisePicker();

    void RecalculateLayout();
    void AddSetToExercise(ActiveExercise& ex);
    void AddExerciseToSession(int exerciseId);

    void handleInputBoxClick(InputBox& box, sf::Vector2i mousePos);
    void handleTyping(InputBox& box, sf::Uint32 unicode);

    string getDayName(int day);

public:
    TrainingState(AppDataRef data);

    void State_Init();
    void State_HandleInput(sf::Event& event);
    void State_Update(float dt);
    void State_Draw(float dt);
};