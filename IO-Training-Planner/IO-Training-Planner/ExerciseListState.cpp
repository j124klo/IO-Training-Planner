#include "DEFINITIONS.hpp"
#include "ExerciseListState.hpp"

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

	float currentY = this->_scrollTop.getPosition().y + this->_scrollTop.getGlobalBounds().height;
	for (size_t i = 0; i < this->_exercises.size(); i++)
	{
		this->_exercises[i]._nameText.setFont(this->_data->assets.getFont("Default Font"));
		this->_exercises[i]._descriptionText.setFont(this->_data->assets.getFont("Default Font"));

		this->_exercises[i]._nameText.setString(this->_exercises[i]._name);
		this->_exercises[i]._descriptionText.setString(this->_exercises[i]._description);
		this->_exercises[i]._nameText.setCharacterSize(40);
		this->_exercises[i]._descriptionText.setCharacterSize(20);
		this->_exercises[i]._nameText.setFillColor(sf::Color::White);
		this->_exercises[i]._descriptionText.setFillColor(sf::Color(200, 200, 200));

		this->_exercises[i]._nameText.setPosition(
			this->_scrollTop.getPosition().x + this->_scrollTop.getGlobalBounds().width * 0.03f,
			currentY
		);
		this->_exercises[i]._descriptionText.setPosition(
			this->_exercises[i]._nameText.getPosition().x,
			this->_exercises[i]._nameText.getPosition().y + this->_exercises[i]._nameText.getGlobalBounds().height + 20.0f
		);

		currentY += this->_exercises[i]._nameText.getGlobalBounds().height + this->_exercises[i]._descriptionText.getGlobalBounds().height + 60.0f;
	}
	this->_scrollBot.setPosition(this->_scrollEdgeTop.getPosition().x, currentY - 60.f);
}
void ExerciseListState::Exercises_Draw(float dt)
{
	for (const auto& ex : this->_exercises)
	{
		this->_data->window.draw(ex._nameText);
		this->_data->window.draw(ex._descriptionText);
	}
}

void ExerciseListState::ScrollPanel_Init()
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
void ExerciseListState::ScrollPanel_HandleInput(sf::Event& event)
{
	this->_data->input.handleScrollEvent(event);
}
void ExerciseListState::ScrollPanel_Update(float dt)
{
	float scrollDelta = this->_data->input.getScrollDelta() * 80.0f;

	float topBound = this->_scrollEdgeTop.getGlobalBounds().height - this->_scrollTop.getGlobalBounds().height / 2.0f;

	float contentDeltaY = this->_scrollTop.getGlobalBounds().height * 1.5f;
	for (size_t i = 0; i < this->_exercises.size(); i++)
	{
		contentDeltaY += this->_exercises[i]._nameText.getGlobalBounds().height + this->_exercises[i]._descriptionText.getGlobalBounds().height + 60.0f;
	}
	float bottomBound = this->_scrollEdgeBot.getPosition().y - contentDeltaY + 60.0f;

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

		float currentY = this->_scrollTop.getPosition().y + this->_scrollTop.getGlobalBounds().height;
		for (size_t i = 0; i < this->_exercises.size(); i++)
		{
			this->_exercises[i]._nameText.setPosition(this->_exercises[i]._nameText.getPosition().x, currentY);
			this->_exercises[i]._descriptionText.setPosition(
				this->_exercises[i]._descriptionText.getPosition().x,
				this->_exercises[i]._nameText.getPosition().y + this->_exercises[i]._nameText.getGlobalBounds().height + 20.0f
			);
			currentY += this->_exercises[i]._nameText.getGlobalBounds().height + this->_exercises[i]._descriptionText.getGlobalBounds().height + 60.0f;

			this->_scrollBot.setPosition(this->_scrollBot.getPosition().x, currentY - 60.f);
		}

		this->_data->input.resetScrollDelta();
	}
}
void ExerciseListState::ScrollPanel_Draw(float dt)
{
	this->_data->window.draw(this->_scrollMid);
	this->_data->window.draw(this->_scrollTop);
	this->_data->window.draw(this->_scrollBot);
	this->_data->window.draw(this->_scrollEdgeTop);
	this->_data->window.draw(this->_scrollEdgeBot);
}

void ExerciseListState::State_Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Default Background"));

	ScrollPanel_Init();
	Exercises_Init();
}
void ExerciseListState::State_HandleInput(sf::Event& event)
{
	ScrollPanel_HandleInput(event);
}
void ExerciseListState::State_Update(float dt)
{
	ScrollPanel_Update(dt);
}
void ExerciseListState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);

	Exercises_Draw(dt);
	ScrollPanel_Draw(dt);
}