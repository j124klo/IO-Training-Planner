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

	LoadAvailablePlans();
	InitSideMenuButtons();
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
	// 1. Obs³uga zamkniêcia okna (Standard)
	if (event.type == sf::Event::Closed) {
		this->_data->window.close();
	}

	// 2. Obs³uga Klikniêcia Lewym Przyciskiem Myszy
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		// Pobieramy pozycjê myszy bezpoœrednio z eventu (to bezpieczniejsze ni¿ getPosition)
		sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

		// --- A. LOGIKA WYBORU PLANU (Choose Plan) ---
		// Iterujemy po liœcie tekstów wyœwietlonych po prawej stronie
		for (size_t i = 0; i < _planButtons.size(); ++i) // Zmieniamy iteracjê na przyciski
		{
			// Sprawdzamy czy myszka jest wewn¹trz prostok¹ta przycisku
			if (_planButtons[i].getGlobalBounds().contains(mousePos))
			{
				int newPlanId = _availablePlans[i].id;

				// Reszta logiki bez zmian...
				if (this->_data->currentUser->currentPlanId == newPlanId) return;

				cout << "[INFO] Zmiana planu na ID: " << newPlanId << endl;
				this->_data->database.assignPlanToUser(this->_data->currentUser->id, newPlanId);
				this->_data->currentUser->currentPlanId = newPlanId;

				Plan_Init();
				return;
			}
		}

		// 2. KLIKNIÊCIE "EDIT PLAN"
		if (_editButton.getGlobalBounds().contains(mousePos))
		{
			this->_isEditMode = !this->_isEditMode; // Prze³¹cznik (ON/OFF)

			cout << "[INFO] Tryb edycji: " << (_isEditMode ? "WLACZONY" : "WYLACZONY") << endl;

			// Prze³adowujemy przyciski, ¿eby zmieniæ kolor napisu i ramki
			InitSideMenuButtons();
			return;
		}

		// 3. KLIKNIÊCIE "SAVE PLAN"
		if (_saveButton.getGlobalBounds().contains(mousePos))
		{
			cout << "[INFO] Zapisywanie zmian w planie..." << endl;

			// Tutaj w przysz³oœci wywo³asz: this->_data->database.updatePlan(...);

			// Efekt wizualny: Wy³¹czamy tryb edycji po zapisie
			if (_isEditMode) {
				_isEditMode = false;
				InitSideMenuButtons();
			}
			return;
		}
	}
}

void PlanState::State_Update(float dt)
{
	// Ewentualne animacje
}

void PlanState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);
	Plan_Draw(dt); // Rysuje tabelê

	// Rysowanie listy planów (Twoja pêtla)
	this->_data->window.draw(_availablePlansTitle);
	for (const auto& btn : _planButtons) this->_data->window.draw(btn);
	for (const auto& txt : _availablePlansTexts) this->_data->window.draw(txt);

	// --- RYSOWANIE BUTTONÓW EDIT / SAVE ---
	this->_data->window.draw(_editButton);
	this->_data->window.draw(_editText);

	this->_data->window.draw(_saveButton);
	this->_data->window.draw(_saveText);
}

void PlanState::LoadAvailablePlans()
{
	// 1. Pobierz dane
	this->_availablePlans = this->_data->database.getAllPlanHeaders();
	this->_availablePlansTexts.clear();
	this->_planButtons.clear(); // Czyœcimy te¿ przyciski

	// 2. Ustawienia pozycji (Po prawej stronie)
	// Zak³adamy, ¿e przycisk ma 220px szerokoœci. 
	// Ustawiamy go 250px od prawej krawêdzi ekranu.
	float buttonWidth = 220.0f;
	float buttonHeight = 50.0f;
	float startX = SCREAN_WIDTH - 250.0f;
	float startY = 150.0f;

	// Kolor z Twojego screenshota (Steel Blue)
	sf::Color steelBlue(77, 153, 204);

	// 3. Tytu³ "CHOOSE PLAN"
	_availablePlansTitle.setFont(this->_data->assets.getFont("Default Font"));
	_availablePlansTitle.setString("CHOOSE PLAN:");
	_availablePlansTitle.setCharacterSize(22);
	_availablePlansTitle.setFillColor(sf::Color::Yellow);

	// Wyœrodkuj tytu³ wzglêdem przycisków
	sf::FloatRect titleBounds = _availablePlansTitle.getLocalBounds();
	_availablePlansTitle.setPosition(startX + (buttonWidth - titleBounds.width) / 2.0f, startY - 40.0f);


	// 4. Pêtla tworz¹ca przyciski
	for (const auto& header : _availablePlans) {

		// --- A. TWORZENIE RAMKI (PRZYCISKU) ---
		sf::RectangleShape button;
		button.setSize(sf::Vector2f(buttonWidth, buttonHeight));
		button.setPosition(startX, startY);

		// Styl ramki
		button.setOutlineThickness(2.0f);
		button.setOutlineColor(steelBlue); // Twój b³êkitny kolor
		button.setFillColor(sf::Color::Transparent); // Œrodek przezroczysty (¿eby by³o widaæ t³o)

		// Logika podœwietlenia aktywnego planu (opcjonalnie: wype³nienie kolorem)
		if (this->_data->currentUser->currentPlanId == header.id) {
			// Jeœli aktywny, np. lekko podœwietl œrodek na zielono albo ramkê na zielono
			button.setOutlineColor(sf::Color::Green);
		}

		// --- B. TWORZENIE TEKSTU ---
		sf::Text text;
		text.setFont(this->_data->assets.getFont("Default Font"));
		text.setString(header.name);
		text.setCharacterSize(18);

		if (this->_data->currentUser->currentPlanId == header.id) {
			text.setFillColor(sf::Color::Green);
		}
		else {
			text.setFillColor(sf::Color::White);
		}

		// Centrowanie tekstu wewn¹trz przycisku
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		text.setPosition(startX + buttonWidth / 2.0f, startY + buttonHeight / 2.0f);

		// --- C. DODANIE DO WEKTORÓW ---
		_planButtons.push_back(button);
		_availablePlansTexts.push_back(text);

		startY += (buttonHeight + 15.0f); // Odstêp miêdzy przyciskami
	}
}

void PlanState::InitSideMenuButtons()
{
	// Konfiguracja wymiarów (takie same jak przyciski planów)
	float buttonWidth = 220.0f;
	float buttonHeight = 50.0f;
	float startX = SCREAN_WIDTH - 250.0f;

	// Ustalamy pozycjê Y pod list¹ planów. 
	// Listê planów zaczynaliœmy od 150.0f. Za³ó¿my, ¿e s¹ max 3-4 plany, 
	// wiêc bezpiecznie damy przyciski funkcyjne na dole ekranu.
	float editY = 600.0f;
	float saveY = 670.0f;

	sf::Font& font = this->_data->assets.getFont("Default Font");

	// --- PRZYCISK EDIT ---
	_editButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
	_editButton.setPosition(startX, editY);
	_editButton.setOutlineThickness(2.0f);
	_editButton.setFillColor(sf::Color::Transparent);

	// Logika koloru Edit: Jeœli jesteœmy w trybie edycji, guzik œwieci
	if (_isEditMode) {
		_editButton.setOutlineColor(sf::Color(255, 165, 0)); // Pomarañczowy
		_editText.setFillColor(sf::Color(255, 165, 0));
		_editText.setString("EDIT MODE ON");
	}
	else {
		_editButton.setOutlineColor(sf::Color::White);
		_editText.setFillColor(sf::Color::White);
		_editText.setString("EDIT PLAN");
	}

	_editText.setFont(font);
	_editText.setCharacterSize(20);
	// Centrowanie tekstu
	sf::FloatRect editRect = _editText.getLocalBounds();
	_editText.setOrigin(editRect.left + editRect.width / 2.0f, editRect.top + editRect.height / 2.0f);
	_editText.setPosition(startX + buttonWidth / 2.0f, editY + buttonHeight / 2.0f);


	// --- PRZYCISK SAVE ---
	_saveButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
	_saveButton.setPosition(startX, saveY);
	_saveButton.setOutlineThickness(2.0f);
	_saveButton.setOutlineColor(sf::Color::Green); // Zielony zawsze
	_saveButton.setFillColor(sf::Color::Transparent);

	_saveText.setFont(font);
	_saveText.setString("SAVE PLAN");
	_saveText.setCharacterSize(20);
	_saveText.setFillColor(sf::Color::Green);

	// Centrowanie tekstu
	sf::FloatRect saveRect = _saveText.getLocalBounds();
	_saveText.setOrigin(saveRect.left + saveRect.width / 2.0f, saveRect.top + saveRect.height / 2.0f);
	_saveText.setPosition(startX + buttonWidth / 2.0f, saveY + buttonHeight / 2.0f);
}