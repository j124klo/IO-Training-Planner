#include "DEFINITIONS.hpp"
#include "RepPercentageState.hpp"
#include "CalculatorState.hpp"

using namespace std;

RepPercentageState::RepPercentageState(AppDataRef data, double oneRM) : GuiState(data), _oneRM(oneRM)
{

}

void RepPercentageState::InitTitle()
{
	this->_titleLabel.setFont(this->_data->assets.getFont("Default Font"));
	this->_titleLabel.setCharacterSize(50);
	this->_titleLabel.setFillColor(sf::Color::Yellow);

	ostringstream oss;
	oss << fixed << setprecision(1) << "1RM: " << this->_oneRM << " kg - Repetition Percentages";
	this->_titleLabel.setString(oss.str());
	this->_titleLabel.setPosition(
		SCREAN_WIDTH / 2.0f - this->_titleLabel.getGlobalBounds().width / 2.0f,
		SCREAN_HEIGHT * 0.08f
	);
}

void RepPercentageState::InitPercentageTable()
{
	// Standardowe procenty 1RM i odpowiadaj¹ce im powtórzenia
	vector<pair<int, string>> percentages = {
		{100, "1 rep"},
		{95, "2 reps"},
		{90, "3-4 reps"},
		{85, "5-6 reps"},
		{80, "7-8 reps"},
		{75, "9-10 reps"},
		{70, "11-12 reps"},
		{65, "13-15 reps"},
		{60, "16-20 reps"},
		{55, "20+ reps"}
	};

	float startY = SCREAN_HEIGHT * 0.18f;
	float rowHeight = 55.0f;
	float leftColumnX = SCREAN_WIDTH * 0.25f;
	float rightColumnX = SCREAN_WIDTH * 0.55f;

	for (size_t i = 0; i < percentages.size(); i++)
	{
		sf::Text percentLabel;
		percentLabel.setFont(this->_data->assets.getFont("Default Font"));
		percentLabel.setCharacterSize(32);
		percentLabel.setFillColor(sf::Color::White);

		ostringstream percentOss;
		percentOss << percentages[i].first << "% (" << percentages[i].second << ")";
		percentLabel.setString(percentOss.str());
		percentLabel.setPosition(leftColumnX, startY + i * rowHeight);
		this->_percentageLabels.push_back(percentLabel);

		sf::Text weightLabel;
		weightLabel.setFont(this->_data->assets.getFont("Default Font"));
		weightLabel.setCharacterSize(32);
		weightLabel.setFillColor(sf::Color::Cyan);

		double weight = this->_oneRM * percentages[i].first / 100.0;
		ostringstream weightOss;
		weightOss << fixed << setprecision(1) << weight << " kg";
		weightLabel.setString(weightOss.str());
		weightLabel.setPosition(rightColumnX, startY + i * rowHeight);
		this->_weightValues.push_back(weightLabel);
	}
}

void RepPercentageState::InitButtons()
{
	this->_backButton._sprite.setTexture(this->_data->assets.getTexture("Continue Button"));

	this->_backButton._pressed = false;
	this->_backButton._update = false;
	this->_backButton._rect = sf::IntRect(0, 0, 800, 300);
	this->_backButton._sprite.setTextureRect(this->_backButton._rect);
	this->_backButton._sprite.setScale(0.25f, 0.25f);
	this->_backButton._sprite.setPosition(
		SCREAN_WIDTH / 2.0f - this->_backButton._sprite.getGlobalBounds().width / 2.0f,
		SCREAN_HEIGHT * 0.88f - this->_backButton._sprite.getGlobalBounds().height / 2.0f
	);
}

void RepPercentageState::State_Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Default Background"));

	InitTitle();
	InitPercentageTable();
	InitButtons();
}

void RepPercentageState::State_HandleInput(sf::Event& event)
{
	if (this->_data->input.isSpriteClickedAndReleased(this->_backButton._sprite, sf::Mouse::Left, this->_data->window, this->_backButton._pressed, this->_backButton._update))
	{
		this->_data->machine.AddState(StateRef(new CalculatorState(this->_data)), true);
	}
}

void RepPercentageState::State_Update(float dt)
{
	if (this->_backButton._update)
	{
		this->_backButton._update = false;

		if (this->_backButton._pressed)
		{
			this->_backButton._rect.left = 800;
			this->_backButton._sprite.setTextureRect(this->_backButton._rect);
		}
		else
		{
			this->_backButton._rect.left = 0;
			this->_backButton._sprite.setTextureRect(this->_backButton._rect);
		}
	}
}

void RepPercentageState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);
	this->_data->window.draw(this->_titleLabel);

	for (const auto& label : this->_percentageLabels)
	{
		this->_data->window.draw(label);
	}
	for (const auto& value : this->_weightValues)
	{
		this->_data->window.draw(value);
	}

	this->_data->window.draw(this->_backButton._sprite);
}