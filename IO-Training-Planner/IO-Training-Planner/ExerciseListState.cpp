#include "DEFINITIONS.hpp"
#include "ExerciseListState.hpp"
#include "HomeState.hpp"
#include "PlanState.hpp"

using namespace std;

ExerciseListState::ExerciseListState(AppDataRef data) : GuiState(data)
{

}

void ExerciseListState::Exercises_Init()
{
	this->_exercises.clear();
	auto dbExercises = this->_data->database.getAllExercises();

	for (const auto& dbEx : dbExercises)
	{
		ExerciseItem localEx;
		localEx._name = dbEx->name;
		localEx._description = dbEx->description;
		this->_exercises.push_back(localEx);
	}

	//	pêtla ustawiaj¹ca teksty na ekranie, ich pozycje oraz style
	for (size_t i = 0; i < this->_exercises.size(); i++)
	{
		this->_exercises[i]._nameText.setFont(this->_data->assets.getFont("Deafult Font"));
		this->_exercises[i]._nameText.setString(this->_exercises[i]._name);
		this->_exercises[i]._nameText.setCharacterSize(40);
		this->_exercises[i]._nameText.setFillColor(sf::Color::White);
		this->_exercises[i]._nameText.setPosition(400.0f, 100.0f + i * 150.0f);
		this->_exercises[i]._descriptionText.setFont(this->_data->assets.getFont("Deafult Font"));
		this->_exercises[i]._descriptionText.setString(this->_exercises[i]._description);
		this->_exercises[i]._descriptionText.setCharacterSize(20);
		this->_exercises[i]._descriptionText.setFillColor(sf::Color(200, 200, 200));
		this->_exercises[i]._descriptionText.setPosition(400.0f, 140.0f + i * 150.0f);
	}
}
void ExerciseListState::Exercises_Draw(float dt)
{
	for (const auto& ex : this->_exercises)
	{
		this->_data->window.draw(ex._nameText);
		this->_data->window.draw(ex._descriptionText);
	}
}

void ExerciseListState::State_Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Deafult Background"));

	Exercises_Init();
}
void ExerciseListState::State_HandleInput()
{
	
}
void ExerciseListState::State_Update(float dt)
{

}
void ExerciseListState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);

	Exercises_Draw(dt);
}