#include "DEFINITIONS.hpp"
#include "PlanState.hpp"
#include <algorithm> // sort
#include <sstream>   // stringstream
#include <iomanip>   // setprecision

using namespace std;

PlanState::PlanState(AppDataRef data) : GuiState(data)
{

}

string PlanState::getDayName(int dayId) {
	switch (dayId) {
	case 1: return "Monday";
	case 2: return "Tuesday";
	case 3: return "Wednesday";
	case 4: return "Thursday";
	case 5: return "Friday";
	case 6: return "Saturday";
	case 7: return "Sunday";
	default: return "";
	}
}

void PlanState::Plan_Init()
{
	this->_planEntries.clear();

	// 1. Sprawdzenie u¿ytkownika i planu
	if (!this->_data->currentUser || !this->_data->currentUser->currentPlanId.has_value()) {
		return;
	}

	int planId = this->_data->currentUser->currentPlanId.value();
	auto dbPlan = this->_data->database.getTrainingPlanById(planId);

	if (!dbPlan) return;

	// 2. Pobieranie danych (Logika)
	for (const auto& dbEntry : dbPlan->entries)
	{
		PlanGUIEntry guiEntry;

		// A. Dane podstawowe
		guiEntry._dayId = dbEntry.dayOfWeek;
		guiEntry._orderIndex = dbEntry.orderInDay; // <-- Wa¿ne: Pobieramy kolejnoœæ z bazy
		guiEntry._dayName = getDayName(dbEntry.dayOfWeek);

		// B. Nazwa æwiczenia
		auto exercise = this->_data->database.getExerciseById(dbEntry.exerciseId);
		if (exercise) {
			guiEntry._exerciseName = exercise->name;
		}
		else {
			guiEntry._exerciseName = "Unknown Exercise";
		}

		// C. Formatowanie celów (Target Info)
		stringstream ss;
		for (size_t i = 0; i < dbEntry.targetValues.size(); ++i) {

			if (holds_alternative<int>(dbEntry.targetValues[i])) {
				ss << get<int>(dbEntry.targetValues[i]);
			}
			else if (holds_alternative<float>(dbEntry.targetValues[i])) {
				float val = get<float>(dbEntry.targetValues[i]);
				if (val == (int)val) ss << (int)val;
				else ss << fixed << setprecision(1) << val;
			}

			if (i < dbEntry.targetValues.size() - 1) {
				ss << " x ";
			}
		}

		// Dopisywanie jednostek (uproszczone)
		if (guiEntry._exerciseName == "Running" || guiEntry._exerciseName == "Cycling") ss << " km/min"; // Dostosuj do swoich potrzeb
		else if (guiEntry._exerciseName.find("Plank") != string::npos) ss << " s";
		else ss << " kg/reps";

		guiEntry._targetInfo = ss.str();

		this->_planEntries.push_back(guiEntry);
	}

	// 3. Sortowanie (Dzieñ -> Kolejnoœæ)
	sort(this->_planEntries.begin(), this->_planEntries.end(),
		[](const PlanGUIEntry& a, const PlanGUIEntry& b) {
			// Najpierw sortuj po dniu tygodnia
			if (a._dayId != b._dayId)
				return a._dayId < b._dayId;

			// Jeœli dzieñ ten sam, sortuj po kolejnoœci æwiczeñ
			return a._orderIndex < b._orderIndex;
		});

	// 4. Konfiguracja Wygl¹du (SFML)
	// Teraz, gdy lista jest posortowana, ustawiamy pozycje na ekranie
	float startX_Day = 350.0f;
	float startX_Name = 550.0f;
	float startX_Target = 900.0f;

	float startY = 150.0f;
	float gapY = 50.0f; // Odstêp miêdzy wierszami

	int lastDayId = -1; // Do sprawdzania czy dzieñ siê powtarza

	for (size_t i = 0; i < this->_planEntries.size(); ++i)
	{
		auto& entry = this->_planEntries[i];
		float currentY = startY + (i * gapY);

		// -- Kolumna 1: Dzieñ --
		entry._dayText.setFont(this->_data->assets.getFont("Default Font")); // Upewnij siê co do nazwy w AssetManagerze
		entry._dayText.setCharacterSize(24);
		entry._dayText.setFillColor(sf::Color(255, 215, 0)); // Z³oty
		entry._dayText.setPosition(startX_Day, currentY);

		// Logika "ukrywania" powtórzonego dnia
		if (entry._dayId != lastDayId) {
			entry._dayText.setString(entry._dayName); // Nowy dzieñ -> wyœwietl nazwê
			lastDayId = entry._dayId;
		}
		else {
			entry._dayText.setString(""); // Ten sam dzieñ -> pusto
		}

		// -- Kolumna 2: Nazwa Æwiczenia --
		entry._nameText.setFont(this->_data->assets.getFont("Default Font"));
		entry._nameText.setString(entry._exerciseName);
		entry._nameText.setCharacterSize(30);
		entry._nameText.setFillColor(sf::Color::White);
		entry._nameText.setPosition(startX_Name, currentY);

		// -- Kolumna 3: Cel --
		entry._targetText.setFont(this->_data->assets.getFont("Default Font"));
		entry._targetText.setString(entry._targetInfo);
		entry._targetText.setCharacterSize(24);
		entry._targetText.setFillColor(sf::Color(200, 200, 200)); // Szary
		entry._targetText.setPosition(startX_Target, currentY + 5.0f); // Lekka korekta Y
	}
}

void PlanState::Plan_Draw(float dt)
{
	// Prosta pêtla rysuj¹ca przygotowane teksty
	for (const auto& entry : this->_planEntries)
	{
		this->_data->window.draw(entry._dayText);
		this->_data->window.draw(entry._nameText);
		this->_data->window.draw(entry._targetText);
	}
}

void PlanState::State_Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Default Background"));
	Plan_Init();
}
void PlanState::State_HandleInput(sf::Event& event)
{
	// Obs³uga powrotu do menu itp.
}

void PlanState::State_Update(float dt)
{
	// Ewentualne animacje
}

void PlanState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);

	// Rysowanie GUI (Menu boczne) - pamiêtaj ¿eby wywo³aæ metodê z GuiState jeœli jej u¿ywasz
	// Gui_Draw(dt); 

	Plan_Draw(dt);
}