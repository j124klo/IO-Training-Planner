#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "GuiState.hpp"
#include "AppPlanner.hpp"
#include "ObjectMoveTo.hpp"
#include "Button.hpp"
#include "Exercise.hpp"
#include "DatabaseManager.hpp"
#include "PlanEntry.hpp"
#include <algorithm>
#include <iomanip>

using namespace std;

// Struktura dla kolumny dnia w tabelce
struct DayColumn {
    int dayId;                              // 1-7 (PON-ND)
    string dayName;                         // "PON", "WT", etc.
    sf::Text headerText;                    // Nag³ówek kolumny
    sf::RectangleShape headerBox;           // T³o nag³ówka
    sf::RectangleShape addButton;           // Przycisk [+] na dole
    sf::Text addButtonText;                 // Tekst "+"
    vector<int> entryIndices;               // Indeksy æwiczeñ z _planEntries dla tego dnia
};

class PlanState : public GuiState
{
private:
    sf::Clock _clock;
    sf::Sprite _background;

    string getDayName(int dayId);
    string getDayShortName(int dayId);      // Nowa: krótka nazwa (PON, WT, ŒR...)
    
    vector<PlanGUIEntry> _planEntries;

    // --- NOWA TABELKA Z KOLUMNAMI ---
    vector<DayColumn> _dayColumns;          // 7 kolumn dla dni tygodnia
    float _columnWidth = 125.0f;            // Szerokoœæ kolumny (7 × 125 = 875px)
    float _tableStartX = 365.0f;            // Przesuniête w prawo o 150px (215 + 150 = 365)
    float _tableStartY = 80.0f;             // Pocz¹tek tabelki Y
    float _headerHeight = 50.0f;            // Wysokoœæ nag³ówka
    float _rowHeight = 35.0f;               // Wysokoœæ wiersza z æwiczeniem (zmniejszona - tylko nazwa)
    
    void InitDayColumns();                  // Inicjalizacja kolumn
    void RefreshTableLayout();              // Odœwie¿enie layoutu po zmianach

    // --- POPUP DO DODAWANIA ÆWICZEÑ ---
    bool _showAddPopup = false;
    int _addPopupDayId = -1;                // Który dzieñ jest wybrany do dodania
    vector<shared_ptr<Exercise>> _allExercises;  // Lista wszystkich æwiczeñ
    vector<sf::Text> _popupExerciseTexts;   // Teksty w popup
    sf::RectangleShape _popupBackground;    // T³o popup
    sf::Text _popupTitle;                   // Tytu³ popup
    
    void ShowAddExercisePopup(int dayId);
    void HideAddExercisePopup();
    void InitPopup();

    // --- KOMUNIKAT O B£ÊDZIE ---
    sf::Text _errorMessage;
    sf::Clock _errorClock;
    bool _showError = false;
    void ShowError(const string& message);
    
    // --- TWORZENIE NOWEGO PLANU ---
    bool _showNewPlanPopup = false;
    string _newPlanName = "";
    sf::RectangleShape _newPlanPopupBg;
    sf::Text _newPlanTitle;
    sf::Text _newPlanInputText;
    sf::RectangleShape _newPlanInputBox;
    sf::RectangleShape _newPlanCreateButton;
    sf::Text _newPlanCreateText;
    sf::RectangleShape _newPlanButton;      // Przycisk "NEW PLAN" w side menu
    sf::Text _newPlanButtonText;
    
    void ShowNewPlanPopup();
    void HideNewPlanPopup();
    void InitNewPlanPopup();
    void CreateNewPlan();
    
    // --- USUWANIE PLANU ---
    bool _showDeleteConfirmPopup = false;
    sf::RectangleShape _deleteConfirmBg;
    sf::Text _deleteConfirmTitle;
    sf::Text _deleteConfirmMessage;
    sf::RectangleShape _deleteYesButton;
    sf::Text _deleteYesText;
    sf::RectangleShape _deleteNoButton;
    sf::Text _deleteNoText;
    sf::RectangleShape _deletePlanButton;   // Przycisk "DELETE PLAN" w side menu
    sf::Text _deletePlanButtonText;
    
    void ShowDeleteConfirmPopup();
    void HideDeleteConfirmPopup();
    void InitDeleteConfirmPopup();
    void DeleteCurrentPlan();
    bool CanDeleteCurrentPlan();  // Sprawdza czy aktualny plan mo¿na usun¹æ

    // --- BOCZNE MENU (plany) ---
    vector<PlanHeader> _availablePlans;
    vector<sf::Text> _availablePlansTexts;
    sf::Text _availablePlansTitle;
    vector<sf::RectangleShape> _planButtons;
    void LoadAvailablePlans();

    // --- PRZYCISKI EDIT & SAVE ---
    sf::RectangleShape _editButton;
    sf::Text _editText;
    sf::RectangleShape _saveButton;
    sf::Text _saveText;
    bool _isEditMode = false;
    void InitSideMenuButtons();

public:
    PlanState(AppDataRef data);

    void Plan_Init();
    void Plan_Draw(float dt);

    void State_Init();
    void State_HandleInput(sf::Event& event);
    void State_Update(float dt);
    void State_Draw(float dt);
};