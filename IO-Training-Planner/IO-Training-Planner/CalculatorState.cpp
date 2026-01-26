#include "DEFINITIONS.hpp"
#include "CalculatorState.hpp"
#include "RepPercentageState.hpp"

using namespace std;

CalculatorState::CalculatorState(AppDataRef data) : GuiState(data)
{

}

//	zmieniæ na zale¿ne od ekranu (* generalnie wszêdzie poprawiæ w tych inicjalizacjach aby pozycje by³y zale¿ne od ekranu)
//	przyk³adowo w osi y niech okienka wpisywania bêd¹ na wysokoœci 2/6 ekranu, 3/6 ekranu, wynik na 4/6 ekranu, a przycisk na 5/6 ekranu
//	a w osi x niech bêd¹ wyœrodkowane na ekranie z przerw¹ oko³o 40,0f miêdzy etykiet¹ a polem wpisywania
void CalculatorState::InputFields_Init()
{
	this->_weightInput.setFont(this->_data->assets.getFont("Default Font"));
	this->_repsInput.setFont(this->_data->assets.getFont("Default Font"));

	// Weight input - pozycja Y na 2/6 ekranu
	this->_weightBounds.setFillColor(sf::Color::White);
	this->_weightBounds.setSize({ 250.0f, 60.0f });
	this->_weightBounds.setPosition(
		SCREAN_WIDTH / 2.0f + 40.0f / 2.0f,
		SCREAN_HEIGHT * 2.0f / 6.0f - this->_weightBounds.getSize().y / 2.0f
	);

	this->_weightInput.setCharacterSize(40);
	this->_weightInput.setFillColor(sf::Color::Black);
	this->_weightInput.setString("");
	this->_weightInput.setPosition(
		this->_weightBounds.getPosition().x + 10.0f,
		this->_weightBounds.getPosition().y + 10.0f
	);
	this->_isWeightActive = false;
	this->_weightText = "";

	// Reps input - pozycja Y na 3/6 ekranu
	this->_repsBounds.setFillColor(sf::Color::White);
	this->_repsBounds.setSize({ 250.0f, 60.0f });
	this->_repsBounds.setPosition(
		SCREAN_WIDTH / 2.0f + 40.0f / 2.0f,
		SCREAN_HEIGHT * 3.0f / 6.0f - this->_repsBounds.getSize().y / 2.0f
	);

	this->_repsInput.setCharacterSize(40);
	this->_repsInput.setFillColor(sf::Color::Black);
	this->_repsInput.setString("");
	this->_repsInput.setPosition(
		this->_repsBounds.getPosition().x + 10.0f,
		this->_repsBounds.getPosition().y + 10.0f
	);
	this->_isRepsActive = false;
	this->_repsText = "";
}
void CalculatorState::Labels_Init()
{
	// Weight label - pozycja Y na 2/6 ekranu, z przerw¹ 40.0f od pola wpisywania
	this->_weightLabel.setFont(this->_data->assets.getFont("Default Font"));
	this->_weightLabel.setCharacterSize(35);
	this->_weightLabel.setFillColor(sf::Color::White);
	this->_weightLabel.setString("Weight (kg):");
	this->_weightLabel.setPosition(
		SCREAN_WIDTH / 2.0f - 40.0f / 2.0f - this->_weightLabel.getGlobalBounds().width,
		SCREAN_HEIGHT * 2.0f / 6.0f - this->_weightLabel.getGlobalBounds().height / 2.0f
	);

	// Reps label - pozycja Y na 3/6 ekranu, z przerw¹ 40.0f od pola wpisywania
	this->_repsLabel.setFont(this->_data->assets.getFont("Default Font"));
	this->_repsLabel.setCharacterSize(35);
	this->_repsLabel.setFillColor(sf::Color::White);
	this->_repsLabel.setString("Reps:");
	this->_repsLabel.setPosition(
		SCREAN_WIDTH / 2.0f - 40.0f / 2.0f - this->_repsLabel.getGlobalBounds().width,
		SCREAN_HEIGHT * 3.0f / 6.0f - this->_repsLabel.getGlobalBounds().height / 2.0f
	);

	// Result value - pozycja Y na 4/6 ekranu, wyœrodkowany na ekranie
	this->_resultValue.setFont(this->_data->assets.getFont("Default Font"));
	this->_resultValue.setCharacterSize(40);
	this->_resultValue.setFillColor(sf::Color::Red);
	this->_resultValue.setString("");
	this->_resultValue.setPosition(
		SCREAN_WIDTH / 2.0f,
		SCREAN_HEIGHT * 4.0f / 6.0f - this->_resultValue.getGlobalBounds().height / 2.0f
	);
}
void CalculatorState::Buttons_Init()
{
	this->_calculateButton._sprite.setTexture(this->_data->assets.getTexture("Continue Button"));

	this->_calculateButton._pressed = false;
	this->_calculateButton._update = false;
	this->_calculateButton._rect = sf::IntRect(0, 0, 800, 300);
	this->_calculateButton._sprite.setTextureRect(this->_calculateButton._rect);
	this->_calculateButton._sprite.setScale(0.25f, 0.25f);
	// Przycisk na 5/6 ekranu, wyœrodkowany
	this->_calculateButton._sprite.setPosition(
		SCREAN_WIDTH / 2.0f - this->_calculateButton._sprite.getGlobalBounds().width / 2.0f,
		SCREAN_HEIGHT * 5.0f / 6.0f - this->_calculateButton._sprite.getGlobalBounds().height / 2.0f
	);
}

double CalculatorState::Calculate1RM(double weight, int reps)
{
	if (reps <= 0 || weight <= 0)
		return 0.0;

	// Brzycki formula: 1RM = (weight * 36) / (37 - reps)
	double denominator = 37.0 - reps;
	if (denominator <= 0)
		return weight; // If reps >= 37, return weight as approximation

	return (weight * 36.0) / denominator;
}

//	zamiast isSpriteClicked u¿yæ isSpriteClickedAndReleased (zmiany dotycz¹ State_HandleInput oraz State_Update). jako wzór mo¿esz skorzystaæ z przycisków GUI w GuiState
void CalculatorState::State_Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Default Background"));

	InputFields_Init();
	Labels_Init();
	Buttons_Init();
}
void CalculatorState::State_HandleInput(sf::Event& event)
{
	// Weight field click
	if (this->_data->input.isRectangleShapeClicked(this->_weightBounds, sf::Mouse::Left, this->_data->window))
	{
		this->_isWeightActive = true;
		this->_isRepsActive = false;
		this->_weightBounds.setOutlineColor(sf::Color::Green);
		this->_weightBounds.setOutlineThickness(2.0f);
		this->_repsBounds.setOutlineThickness(0.0f);
	}
	// Reps field click
	else if (this->_data->input.isRectangleShapeClicked(this->_repsBounds, sf::Mouse::Left, this->_data->window))
	{
		this->_isWeightActive = false;
		this->_isRepsActive = true;
		this->_repsBounds.setOutlineColor(sf::Color::Green);
		this->_repsBounds.setOutlineThickness(2.0f);
		this->_weightBounds.setOutlineThickness(0.0f);
	}
	// Calculate button click
	else if (this->_data->input.isSpriteClickedAndReleased(this->_calculateButton._sprite, sf::Mouse::Left, this->_data->window, this->_calculateButton._pressed, this->_calculateButton._update))
	{
		if (!this->_weightText.empty() && !this->_repsText.empty())
		{
			try
			{
				double weight = stod(this->_weightText);
				int reps = stoi(this->_repsText);
				double oneRM = Calculate1RM(weight, reps);

				// PrzejdŸ do widoku procentów powtórzeñ
				this->_data->machine.AddState(StateRef(new RepPercentageState(this->_data, oneRM)), true);
			}
			catch (const exception&)
			{
				this->_resultValue.setString("Invalid input");
				this->_resultValue.setFillColor(sf::Color::Red);
				this->_resultValue.setOrigin(this->_resultValue.getGlobalBounds().width / 2.0f, 0);
			}
		}
		else
		{
			this->_resultValue.setString("Fill all fields");
			this->_resultValue.setFillColor(sf::Color::Red);
			this->_resultValue.setOrigin(this->_resultValue.getGlobalBounds().width / 2.0f, 0);
		}

		this->_isWeightActive = false;
		this->_isRepsActive = false;
		this->_weightBounds.setOutlineThickness(0.0f);
		this->_repsBounds.setOutlineThickness(0.0f);
	}
	// Click elsewhere to deselect
	else if (this->_data->input.isSpriteClicked(this->_background, sf::Mouse::Left, this->_data->window))
	{
		this->_isWeightActive = false;
		this->_isRepsActive = false;
		this->_weightBounds.setOutlineThickness(0.0f);
		this->_repsBounds.setOutlineThickness(0.0f);
	}

	// Text input for weight
	if (this->_data->input.typingText(event, this->_weightText, this->_weightInput, this->_isWeightActive))
	{
		this->_weightInput.setPosition(
			this->_weightBounds.getPosition().x + 10.0f,
			this->_weightBounds.getPosition().y + 10.0f
		);
	}
	// Text input for reps
	else if (this->_data->input.typingText(event, this->_repsText, this->_repsInput, this->_isRepsActive))
	{
		this->_repsInput.setPosition(
			this->_repsBounds.getPosition().x + 10.0f,
			this->_repsBounds.getPosition().y + 10.0f
		);
	}
}
void CalculatorState::State_Update(float dt)
{
	if (this->_calculateButton._update)
	{
		this->_calculateButton._update = false;

		if (this->_calculateButton._pressed)
		{
			this->_calculateButton._rect.left = 800;
			this->_calculateButton._sprite.setTextureRect(this->_calculateButton._rect);
		}
		else
		{
			this->_calculateButton._rect.left = 0;
			this->_calculateButton._sprite.setTextureRect(this->_calculateButton._rect);
		}
	}
}
void CalculatorState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);

	// Draw labels
	this->_data->window.draw(this->_weightLabel);
	this->_data->window.draw(this->_repsLabel);

	// Draw input fields
	this->_data->window.draw(this->_weightBounds);
	this->_data->window.draw(this->_weightInput);
	this->_data->window.draw(this->_repsBounds);
	this->_data->window.draw(this->_repsInput);

	// Draw result
	this->_data->window.draw(this->_resultValue);

	// Draw button
	this->_data->window.draw(this->_calculateButton._sprite);
}