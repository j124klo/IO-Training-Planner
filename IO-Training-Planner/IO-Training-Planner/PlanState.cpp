#include "DEFINITIONS.hpp"
#include "PlanState.hpp"
#include "ExerciseListState.hpp"
#include "HomeState.hpp"

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

	if (!this->_data->currentUser || !this->_data->currentUser->currentPlanId.has_value()) {
		return;
	}

	int planId = this->_data->currentUser->currentPlanId.value();

	auto dbPlan = this->_data->database.getTrainingPlanById(planId);

	if (!dbPlan) return;	//	<-- nie ma szans ¿e to tak zostanie bo bêdzie crash programu

	for (const auto& dbEntry : dbPlan->entries)
	{
		PlanGUIEntry guiEntry;

		guiEntry._dayId = dbEntry.dayOfWeek;
		guiEntry._dayName = getDayName(dbEntry.dayOfWeek);

		auto exercise = this->_data->database.getExerciseById(dbEntry.exerciseId);
		if (exercise) {
			guiEntry._exerciseName = exercise->name;
		}
		else {
			guiEntry._exerciseName = "Nieznane cwiczenie";
		}

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

		if (guiEntry._exerciseName == "Bieganie") ss << " km";
		else if (guiEntry._exerciseName.find("Push-ups") != string::npos) ss << " powt.";
		else ss << " kg";

		guiEntry._targetInfo = ss.str();

		this->_planEntries.push_back(guiEntry);
	}

	sort(this->_planEntries.begin(), this->_planEntries.end(),
		[](const PlanGUIEntry& a, const PlanGUIEntry& b) {
			return a._dayId < b._dayId;
		});


}
void PlanState::Plan_Draw(float dt)
{
	/*
	for (const auto& ex : this->_exercises)
	{
		this->_data->window.draw(ex._nameText);
		this->_data->window.draw(ex._descriptionText);
	}
	*/

}

void PlanState::State_Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Deafult Background"));

	Plan_Init();
}
void PlanState::State_HandleInput()
{

}
void PlanState::State_Update(float dt)
{
	
}
void PlanState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);

	Plan_Draw(dt);
}