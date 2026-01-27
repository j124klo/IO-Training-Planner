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
string PlanState::getDayShortName(int dayId) {
	switch (dayId) {
	case 1: return "MON";
	case 2: return "TUE";
	case 3: return "WED";
	case 4: return "THU";
	case 5: return "FRI";
	case 6: return "SAT";
	case 7: return "SUN";
	default: return "?";
	}
}

void PlanState::Plan_Init()
{
	this->_planEntries.clear();
	_tableScrollOffset = 0;  // Reset scrollowania tabelki

	// Zawsze inicjalizuj kolumny i przyciski - nawet jeœli nie ma planu
	InitDayColumns();

	// Sprawdzenie u¿ytkownika
	if (!this->_data->currentUser) {
		RefreshTableLayout();
		LoadAvailablePlans();
		InitSideMenuButtons();
		InitLeftMenuButtons();
		return;
	}

	if (!this->_data->currentUser->currentPlanId.has_value()) {
		RefreshTableLayout();
		LoadAvailablePlans();
		InitSideMenuButtons();
		InitLeftMenuButtons();
		return;
	}

	int planId = this->_data->currentUser->currentPlanId.value();
	auto dbPlan = this->_data->database.getTrainingPlanById(planId);

	if (!dbPlan) {
		// Plan nie existe - wyczyœæ przypisanie
		this->_data->currentUser->currentPlanId = nullopt;
		RefreshTableLayout();
		LoadAvailablePlans();
		InitSideMenuButtons();
		InitLeftMenuButtons();
		return;
	}

	// Pobieranie danych z planu
	for (const auto& dbEntry : dbPlan->entries)
	{
		PlanGUIEntry guiEntry;

		guiEntry._exerciseId = dbEntry.exerciseId;
		guiEntry._targets = dbEntry.targetValues;
		guiEntry._dayId = dbEntry.dayOfWeek;
		guiEntry._orderIndex = dbEntry.orderInDay;
		guiEntry._dayName = getDayName(dbEntry.dayOfWeek);

		auto exercise = this->_data->database.getExerciseById(dbEntry.exerciseId);
		if (exercise) {
			guiEntry._exerciseName = exercise->name;
		}
		else {
			guiEntry._exerciseName = "Unknown";
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
		guiEntry._targetInfo = ss.str();

		this->_planEntries.push_back(guiEntry);
	}

	// Sortowanie
	sort(this->_planEntries.begin(), this->_planEntries.end(),
		[](const PlanGUIEntry& a, const PlanGUIEntry& b) {
			if (a._dayId != b._dayId)
				return a._dayId < b._dayId;
			return a._orderIndex < b._orderIndex;
		});

	// Odœwie¿ layout
	RefreshTableLayout();
	LoadAvailablePlans();
	InitSideMenuButtons();
	InitLeftMenuButtons();
}
void PlanState::Plan_Draw(float dt)
{
	// 1. Rysuj nag³ówki kolumn (dni tygodnia)
	for (const auto& col : _dayColumns)
	{
		this->_data->window.draw(col.headerBox);
		this->_data->window.draw(col.headerText);

		// Rysuj przycisk [+] tylko w trybie edycji
		if (_isEditMode) {
			this->_data->window.draw(col.addButton);
			this->_data->window.draw(col.addButtonText);
		}
	}

	// 2. Rysuj æwiczenia w kolumnach (z uwzglêdnieniem scrollowania)
	float contentStartY = _tableStartY + _headerHeight + 10.0f;
	float tableBottom = contentStartY + _tableMaxVisibleRows * _rowHeight;

	for (const auto& entry : _planEntries)
	{
		// SprawdŸ czy element jest w widocznym obszarze
		float entryY = entry._nameText.getPosition().y;
		if (entryY >= contentStartY - _rowHeight && entryY < tableBottom)
		{
			this->_data->window.draw(entry._nameText);
		}
	}

	// 3. Rysuj scrollbar tabelki (jeœli potrzebny)
	int maxRows = GetMaxTableRows();
	if (maxRows > _tableMaxVisibleRows)
	{
		this->_data->window.draw(_tableScrollbar);
		this->_data->window.draw(_tableScrollThumb);
	}

	// 4. Rysuj popup (jeœli widoczny)
	if (_showAddPopup)
	{
		this->_data->window.draw(_popupBackground);
		this->_data->window.draw(_popupTitle);

		// Rysuj tylko widoczne æwiczenia (z uwzglêdnieniem scrollowania)
		float startY = _popupBackground.getPosition().y + 50.0f;
		float startX = _popupBackground.getPosition().x + 15.0f;
		float itemHeight = 32.0f;
		float popupBottom = _popupBackground.getPosition().y + _popupBackground.getSize().y - 15.0f;

		for (int i = _popupScrollOffset; i < (int)_popupExerciseTexts.size() && i < _popupScrollOffset + _popupMaxVisibleItems; ++i)
		{
			float posY = startY + (i - _popupScrollOffset) * itemHeight;

			// SprawdŸ czy element mieœci siê w popup
			if (posY + itemHeight < popupBottom)
			{
				_popupExerciseTexts[i].setPosition(startX, posY);
				this->_data->window.draw(_popupExerciseTexts[i]);
			}
		}

		// Rysuj scrollbar tylko jeœli jest wiêcej elementów ni¿ widocznych
		if (_allExercises.size() > (size_t)_popupMaxVisibleItems)
		{
			this->_data->window.draw(_popupScrollbar);
			this->_data->window.draw(_popupScrollThumb);
		}
	}
}

void PlanState::State_Init()
{
	this->_background.setTexture(this->_data->assets.getTexture("Default Background"));
	InitPopup();              // Popup dodawania æwiczeñ
	InitNewPlanPopup();       // Popup tworzenia nowego planu
	InitDeleteConfirmPopup(); // Popup potwierdzenia usuniêcia
	InitPlanListScrollbar();  // Scrollbar listy planów
	Plan_Init();
}
void PlanState::State_HandleInput(sf::Event& event)
{
	// 1. Obs³uga zamkniêcia okna (Standard)
	if (event.type == sf::Event::Closed) {
		this->_data->window.close();
	}

	// --- Obs³uga scrollowania kó³kiem myszy ---
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		sf::Vector2f mousePos(static_cast<float>(event.mouseWheelScroll.x),
			static_cast<float>(event.mouseWheelScroll.y));
		int delta = (event.mouseWheelScroll.delta > 0) ? -1 : 1;

		// Scrollowanie w popup dodawania æwiczeñ
		if (_showAddPopup && _popupBackground.getGlobalBounds().contains(mousePos))
		{
			UpdatePopupScroll(delta);
			return;
		}

		// Scrollowanie listy planów (prawa strona)
		if (_planListArea.contains(mousePos))
		{
			UpdatePlanListScroll(delta);
			return;
		}

		// Scrollowanie tabelki æwiczeñ (obszar tabelki)
		float tableRight = _tableStartX + 7 * _columnWidth;
		float tableBottom = _tableStartY + _headerHeight + _tableMaxVisibleRows * _rowHeight + 100.0f;
		sf::FloatRect tableArea(_tableStartX, _tableStartY, tableRight - _tableStartX, tableBottom - _tableStartY);

		if (tableArea.contains(mousePos))
		{
			UpdateTableScroll(delta);
			return;
		}
	}

	// --- Obs³uga wpisywania tekstu dla nowego planu ---
	if (_showNewPlanPopup && event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode == 8) { // Backspace
			if (!_newPlanName.empty()) {
				_newPlanName.pop_back();
			}
		}
		else if (event.text.unicode >= 32 && event.text.unicode < 128) {
			if (_newPlanName.length() < 30) { // Limit 30 znaków
				_newPlanName += static_cast<char>(event.text.unicode);
			}
		}
		_newPlanInputText.setString(_newPlanName);
		return;
	}

	// 2. Obs³uga Klikniêcia Lewym Przyciskiem Myszy
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

		// --- POPUP: Obs³uga klikniêæ w popup (jeœli widoczny) ---
		if (_showAddPopup)
		{
			// SprawdŸ czy klikniêto poza popup (zamknij)
			if (!_popupBackground.getGlobalBounds().contains(mousePos))
			{
				HideAddExercisePopup();
				return;
			}

			// SprawdŸ czy klikniêto na æwiczenie w liœcie (z uwzglêdnieniem scrollowania)
			float startY = _popupBackground.getPosition().y + 50.0f;
			float startX = _popupBackground.getPosition().x + 15.0f;
			float itemHeight = 32.0f;
			float itemWidth = _popupBackground.getSize().x - 40.0f;  // Bez scrollbara

			for (int i = _popupScrollOffset; i < (int)_allExercises.size() && i < _popupScrollOffset + _popupMaxVisibleItems; ++i)
			{
				float posY = startY + (i - _popupScrollOffset) * itemHeight;
				sf::FloatRect itemBounds(startX, posY, itemWidth, itemHeight);

				if (itemBounds.contains(mousePos))
				{
					auto& exercise = _allExercises[i];

					// BLOKADA DUPLIKATÓW: SprawdŸ czy to æwiczenie ju¿ jest w tym dniu
					bool alreadyExists = false;
					for (const auto& entry : _planEntries) {
						if (entry._dayId == _addPopupDayId && entry._exerciseId == exercise->id) {
							alreadyExists = true;
							break;
						}
					}

					if (alreadyExists) {
						ShowError("This exercise is already in this day!");
						HideAddExercisePopup();
						return;
					}

					// Dodaj æwiczenie do planu
					PlanGUIEntry newEntry;
					newEntry._exerciseId = exercise->id;
					newEntry._dayId = _addPopupDayId;
					newEntry._dayName = getDayName(_addPopupDayId);
					newEntry._exerciseName = exercise->name;
					newEntry._orderIndex = 99;
					newEntry._targetInfo = "0";
					newEntry._targets = { 0 };

					_planEntries.push_back(newEntry);

					HideAddExercisePopup();
					RefreshTableLayout();
					return;
				}
			}
			return;
		}

		// --- POPUP: Tworzenie nowego planu ---
		if (_showNewPlanPopup)
		{
			// Klikniêcie poza popup - zamknij
			if (!_newPlanPopupBg.getGlobalBounds().contains(mousePos))
			{
				HideNewPlanPopup();
				return;
			}

			// Klikniêcie na przycisk CREATE
			if (_newPlanCreateButton.getGlobalBounds().contains(mousePos))
			{
				CreateNewPlan();
				return;
			}
			return;
		}

		// --- POPUP: Potwierdzenie usuniêcia planu ---
		if (_showDeleteConfirmPopup)
		{
			// Klikniêcie na YES - usuñ plan
			if (_deleteYesButton.getGlobalBounds().contains(mousePos))
			{
				DeleteCurrentPlan();
				return;
			}

			// Klikniêcie na NO lub poza popup - zamknij
			if (_deleteNoButton.getGlobalBounds().contains(mousePos) ||
				!_deleteConfirmBg.getGlobalBounds().contains(mousePos))
			{
				HideDeleteConfirmPopup();
				return;
			}
			return;
		}

		// --- A. LOGIKA WYBORU PLANU (Choose Plan) - z uwzglêdnieniem scrollowania ---
		for (size_t i = 0; i < _planButtons.size(); ++i)
		{
			// Sprawdzamy czy myszka jest wewn¹trz prostok¹ta przycisku
			if (_planButtons[i].getGlobalBounds().contains(mousePos))
			{
				int actualIndex = static_cast<int>(i) + _planListScrollOffset;
				if (actualIndex >= 0 && actualIndex < static_cast<int>(_availablePlans.size()))
				{
					int newPlanId = _availablePlans[actualIndex].id;

					if (this->_data->currentUser->currentPlanId == newPlanId) return;

					this->_data->database.assignPlanToUser(this->_data->currentUser->id, newPlanId);
					this->_data->currentUser->currentPlanId = newPlanId;

					Plan_Init();
				}
				return;
			}
		}

		// 2. KLIKNIÊCIE "EDIT PLAN"
		if (_editButton.getGlobalBounds().contains(mousePos))
		{
			if (!CanEditCurrentPlan()) {
				ShowError("Cannot edit public plans!");
				return;
			}

			this->_isEditMode = !this->_isEditMode; // Prze³¹cznik (ON/OFF)

			// Prze³adowujemy przyciski, ¿eby zmieniæ kolor napisu i ramki
			InitSideMenuButtons();
			RefreshTableLayout(); // Odœwie¿ aby pokazaæ/ukryæ przyciski [+]
			return;
		}

		// 3. KLIKNIÊCIE "SAVE PLAN"
		if (_saveButton.getGlobalBounds().contains(mousePos))
		{
			if (!CanEditCurrentPlan()) {
				ShowError("Cannot save changes to public plans!");
				return;
			}

			if (this->_data->currentUser && this->_data->currentUser->currentPlanId.has_value())
			{
				int planId = this->_data->currentUser->currentPlanId.value();
				bool success = this->_data->database.updatePlanEntries(planId, _planEntries);
			}

			if (_isEditMode) {
				_isEditMode = false;
				InitSideMenuButtons();
			}
			return;
		}

		// --- KLIKNIÊCIE "NEW PLAN" (teraz po lewej stronie) ---
		if (_newPlanButton.getGlobalBounds().contains(mousePos))
		{
			ShowNewPlanPopup();
			return;
		}

		// --- KLIKNIÊCIE "DELETE PLAN" (teraz po lewej stronie) ---
		if (_deletePlanButton.getGlobalBounds().contains(mousePos))
		{
			if (CanDeleteCurrentPlan()) {
				ShowDeleteConfirmPopup();
			}
			else {
				ShowError("Cannot delete public plans!");
			}
			return;
		}

		// --- D. KLIKNIÊCIE NA PRZYCISKA [+] (tylko w trybie edycji) ---
		if (_isEditMode)
		{
			for (const auto& col : _dayColumns)
			{
				if (col.addButton.getGlobalBounds().contains(mousePos))
				{
					ShowAddExercisePopup(col.dayId);
					return;
				}
			}

			// --- E. USUWANIE ÆWICZEÑ (klikniêcie na æwiczenie) ---
			for (int i = static_cast<int>(_planEntries.size()) - 1; i >= 0; --i)
			{
				if (_planEntries[i]._nameText.getGlobalBounds().contains(mousePos))
				{
					_planEntries.erase(_planEntries.begin() + i);
					RefreshTableLayout();
					return;
				}
			}
		}
	}
}
void PlanState::State_Update(float dt)
{
	// Jeœli zmieni³ siê tryb edycji, mo¿emy zaktualizowaæ kolory
	// (Dla wydajnoœci mo¿na to robiæ tylko przy klikniêciu Edit, ale tu jest proœciej)

	for (auto& entry : _planEntries) {
		if (_isEditMode) {
			entry._nameText.setFillColor(sf::Color::Red); // W trybie edycji na czerwono
		}
		else {
			entry._nameText.setFillColor(sf::Color::White); // Normalnie na bia³o
		}
	}
}
void PlanState::State_Draw(float dt)
{
	this->_data->window.draw(this->_background);
	Plan_Draw(dt);

	// Rysowanie listy planów (po prawej)
	this->_data->window.draw(_availablePlansTitle);
	for (const auto& btn : _planButtons) this->_data->window.draw(btn);
	for (const auto& txt : _availablePlansTexts) this->_data->window.draw(txt);
	
	// Rysuj scrollbar listy planów (jeœli potrzebny)
	if (_availablePlans.size() > (size_t)_planListMaxVisible)
	{
		this->_data->window.draw(_planListScrollbar);
		this->_data->window.draw(_planListScrollThumb);
	}

	// --- RYSOWANIE WSZYSTKICH PRZYCISKÓW (po lewej stronie) ---
	this->_data->window.draw(_newPlanButton);
	this->_data->window.draw(_newPlanButtonText);
	
	this->_data->window.draw(_deletePlanButton);
	this->_data->window.draw(_deletePlanButtonText);
	
	this->_data->window.draw(_editButton);
	this->_data->window.draw(_editText);

	this->_data->window.draw(_saveButton);
	this->_data->window.draw(_saveText);

	// --- KOMUNIKAT O B£ÊDZIE (znika po 3 sekundach) ---
	if (_showError) {
		if (_errorClock.getElapsedTime().asSeconds() < 3.0f) {
			this->_data->window.draw(_errorMessage);
		}
		else {
			_showError = false;
		}
	}

	// --- POPUP DO DODAWANIA ÆWICZEÑ ---
	if (_showAddPopup)
	{
		this->_data->window.draw(_popupBackground);
		this->_data->window.draw(_popupTitle);

		// Rysuj tylko widoczne æwiczenia (z uwzglêdnieniem scrollowania)
		float startY = _popupBackground.getPosition().y + 50.0f;
		float startX = _popupBackground.getPosition().x + 15.0f;
		float itemHeight = 32.0f;
		float popupBottom = _popupBackground.getPosition().y + _popupBackground.getSize().y - 15.0f;

		for (int i = _popupScrollOffset; i < (int)_popupExerciseTexts.size() && i < _popupScrollOffset + _popupMaxVisibleItems; ++i)
		{
			float posY = startY + (i - _popupScrollOffset) * itemHeight;

			// SprawdŸ czy element mieœci siê w popup
			if (posY + itemHeight < popupBottom)
			{
				_popupExerciseTexts[i].setPosition(startX, posY);
				this->_data->window.draw(_popupExerciseTexts[i]);
			}
		}

		// Rysuj scrollbar tylko jeœli jest wiêcej elementów ni¿ widocznych
		if (_allExercises.size() > (size_t)_popupMaxVisibleItems)
		{
			this->_data->window.draw(_popupScrollbar);
			this->_data->window.draw(_popupScrollThumb);
		}
	}

	// --- POPUP DO TWORZENIA NOWEGO PLANU ---
	if (_showNewPlanPopup)
	{
		this->_data->window.draw(_newPlanPopupBg);
		this->_data->window.draw(_newPlanTitle);
		this->_data->window.draw(_newPlanInputBox);
		this->_data->window.draw(_newPlanInputText);
		this->_data->window.draw(_newPlanCreateButton);
		this->_data->window.draw(_newPlanCreateText);
	}

	// --- POPUP POTWIERDZENIA USUNIÊCIA ---
	if (_showDeleteConfirmPopup)
	{
		this->_data->window.draw(_deleteConfirmBg);
		this->_data->window.draw(_deleteConfirmTitle);
		this->_data->window.draw(_deleteConfirmMessage);
		this->_data->window.draw(_deleteYesButton);
		this->_data->window.draw(_deleteYesText);
		this->_data->window.draw(_deleteNoButton);
		this->_data->window.draw(_deleteNoText);
	}
}

void PlanState::LoadAvailablePlans()
{
	// 1. Pobierz dane - tylko plany dostêpne dla tego u¿ytkownika
	if (this->_data->currentUser) {
		this->_availablePlans = this->_data->database.getPlansForUser(this->_data->currentUser->id);
	}
	else {
		this->_availablePlans = this->_data->database.getAllPlanHeaders();
	}

	this->_availablePlansTexts.clear();
	this->_planButtons.clear();

	// 2. Ustalenia pozycji (Po prawej stronie)
	float buttonWidth = 220.0f;
	float buttonHeight = 50.0f;
	float startX = SCREAN_WIDTH - 250.0f;
	float startY = 150.0f;

	sf::Color steelBlue(77, 153, 204);

	// 3. Tytu³ "CHOOSE PLAN"
	_availablePlansTitle.setFont(this->_data->assets.getFont("Default Font"));
	_availablePlansTitle.setString("CHOOSE PLAN:");
	_availablePlansTitle.setCharacterSize(22);
	_availablePlansTitle.setFillColor(sf::Color::Yellow);

	sf::FloatRect titleBounds = _availablePlansTitle.getLocalBounds();
	_availablePlansTitle.setPosition(startX + (buttonWidth - titleBounds.width) / 2.0f, startY - 40.0f);

	// 4. Ustaw obszar listy planów (dla scrollowania)
	_planListArea = sf::FloatRect(startX - 10.0f, startY - 50.0f, buttonWidth + 50.0f,
		_planListMaxVisible * (buttonHeight + 15.0f) + 60.0f);

	// 5. Pêtla tworz¹ca przyciski (tylko widoczne elementy)
	int visibleCount = min(static_cast<int>(_availablePlans.size()) - _planListScrollOffset, _planListMaxVisible);

	for (int i = 0; i < visibleCount; ++i) {
		int actualIndex = i + _planListScrollOffset;
		const auto& header = _availablePlans[actualIndex];

		sf::RectangleShape button;
		button.setSize(sf::Vector2f(buttonWidth, buttonHeight));
		button.setPosition(startX, startY + i * (buttonHeight + 15.0f));

		button.setOutlineThickness(2.0f);
		button.setOutlineColor(steelBlue);
		button.setFillColor(sf::Color::Transparent);

		if (this->_data->currentUser && this->_data->currentUser->currentPlanId == header.id) {
			button.setOutlineColor(sf::Color::Green);
		}

		sf::Text text;
		text.setFont(this->_data->assets.getFont("Default Font"));
		text.setString(header.name);
		text.setCharacterSize(18);

		if (this->_data->currentUser && this->_data->currentUser->currentPlanId == header.id) {
			text.setFillColor(sf::Color::Green);
		}
		else {
			text.setFillColor(sf::Color::White);
		}

		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		text.setPosition(startX + buttonWidth / 2.0f, startY + i * (buttonHeight + 15.0f) + buttonHeight / 2.0f);

		_planButtons.push_back(button);
		_availablePlansTexts.push_back(text);
	}

	// 6. Aktualizuj scrollbar listy planów
	if (_availablePlans.size() > (size_t)_planListMaxVisible)
	{
		float scrollbarHeight = _planListMaxVisible * (buttonHeight + 15.0f);
		_planListScrollbar.setSize(sf::Vector2f(10.0f, scrollbarHeight));
		_planListScrollbar.setPosition(startX + buttonWidth + 10.0f, startY);
		_planListScrollbar.setFillColor(sf::Color(60, 60, 60));

		float visibleRatio = (float)_planListMaxVisible / (float)_availablePlans.size();
		float thumbHeight = scrollbarHeight * visibleRatio;
		if (thumbHeight < 30.0f) thumbHeight = 30.0f;

		_planListScrollThumb.setSize(sf::Vector2f(10.0f, thumbHeight));
		_planListScrollThumb.setFillColor(sf::Color(120, 120, 120));

		// Pozycja kciuka
		int maxOffset = static_cast<int>(_availablePlans.size()) - _planListMaxVisible;
		float scrollRange = scrollbarHeight - thumbHeight;
		float scrollRatio = (maxOffset > 0) ? (float)_planListScrollOffset / (float)maxOffset : 0.0f;
		_planListScrollThumb.setPosition(startX + buttonWidth + 10.0f, startY + scrollRange * scrollRatio);
	}
}

void PlanState::InitPlanListScrollbar()
{
	_planListScrollOffset = 0;
	_planListScrollbar.setFillColor(sf::Color(60, 60, 60));
	_planListScrollThumb.setFillColor(sf::Color(120, 120, 120));
}
void PlanState::UpdatePlanListScroll(int delta)
{
	int maxOffset = static_cast<int>(_availablePlans.size()) - _planListMaxVisible;
	if (maxOffset < 0) maxOffset = 0;

	_planListScrollOffset += delta;
	if (_planListScrollOffset < 0) _planListScrollOffset = 0;
	if (_planListScrollOffset > maxOffset) _planListScrollOffset = maxOffset;

	// Prze³aduj widoczne przyciski
	LoadAvailablePlans();
}

void PlanState::InitSideMenuButtons()
{
	// Pusta funkcja - przyciski przeniesione do InitLeftMenuButtons
}
void PlanState::InitLeftMenuButtons()
{
	// Konfiguracja wymiarów
	float buttonWidth = 180.0f;
	float buttonHeight = 50.0f;
	float startX = this->_availablePlansTitle.getPosition().x + this->_availablePlansTitle.getGlobalBounds().width / 2 - 90.0f;   // Przesuniête w prawo (by³o 20)
	float newPlanY = 500.0f;    // Przesuniête w dó³ (by³o 150)
	float deletePlanY = 570.0f; // Przesuniête w dó³ (by³o 220)
	float editY = 640.0f;       // EDIT pod DELETE
	float saveY = 710.0f;       // SAVE pod EDIT

	sf::Font& font = this->_data->assets.getFont("Default Font");

	// --- PRZYCISK NEW PLAN ---
	_newPlanButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
	_newPlanButton.setPosition(startX, newPlanY);
	_newPlanButton.setOutlineThickness(2.0f);
	_newPlanButton.setOutlineColor(sf::Color::Cyan);
	_newPlanButton.setFillColor(sf::Color::Transparent);

	_newPlanButtonText.setFont(font);
	_newPlanButtonText.setString("NEW PLAN");
	_newPlanButtonText.setCharacterSize(20);
	_newPlanButtonText.setFillColor(sf::Color::Cyan);
	sf::FloatRect newPlanRect = _newPlanButtonText.getLocalBounds();
	_newPlanButtonText.setOrigin(newPlanRect.left + newPlanRect.width / 2.0f, newPlanRect.top + newPlanRect.height / 2.0f);
	_newPlanButtonText.setPosition(startX + buttonWidth / 2.0f, newPlanY + buttonHeight / 2.0f);

	// --- PRZYCISK DELETE PLAN ---
	_deletePlanButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
	_deletePlanButton.setPosition(startX, deletePlanY);
	_deletePlanButton.setOutlineThickness(2.0f);
	_deletePlanButton.setFillColor(sf::Color::Transparent);
	
	if (CanDeleteCurrentPlan()) {
		_deletePlanButton.setOutlineColor(sf::Color::Red);
		_deletePlanButtonText.setFillColor(sf::Color::Red);
	} else {
		_deletePlanButton.setOutlineColor(sf::Color(80, 80, 80));
		_deletePlanButtonText.setFillColor(sf::Color(80, 80, 80));
	}

	_deletePlanButtonText.setFont(font);
	_deletePlanButtonText.setString("DELETE PLAN");
	_deletePlanButtonText.setCharacterSize(20);
	sf::FloatRect deleteRect = _deletePlanButtonText.getLocalBounds();
	_deletePlanButtonText.setOrigin(deleteRect.left + deleteRect.width / 2.0f, deleteRect.top + deleteRect.height / 2.0f);
	_deletePlanButtonText.setPosition(startX + buttonWidth / 2.0f, deletePlanY + buttonHeight / 2.0f);

	// --- PRZYCISK EDIT ---
	_editButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
	_editButton.setPosition(startX, editY);
	_editButton.setOutlineThickness(2.0f);
	_editButton.setFillColor(sf::Color::Transparent);

	if (!CanEditCurrentPlan()) {
		_editButton.setOutlineColor(sf::Color(80, 80, 80));
		_editText.setFillColor(sf::Color(80, 80, 80));
		_editText.setString("EDIT PLAN");
		_isEditMode = false;
	}
	else if (_isEditMode) {
		_editButton.setOutlineColor(sf::Color(255, 165, 0));
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
	sf::FloatRect editRect = _editText.getLocalBounds();
	_editText.setOrigin(editRect.left + editRect.width / 2.0f, editRect.top + editRect.height / 2.0f);
	_editText.setPosition(startX + buttonWidth / 2.0f, editY + buttonHeight / 2.0f);

	// --- PRZYCISK SAVE ---
	_saveButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
	_saveButton.setPosition(startX, saveY);
	_saveButton.setOutlineThickness(2.0f);
	_saveButton.setFillColor(sf::Color::Transparent);

	if (!CanEditCurrentPlan()) {
		_saveButton.setOutlineColor(sf::Color(80, 80, 80));
		_saveText.setFillColor(sf::Color(80, 80, 80));
	} else {
		_saveButton.setOutlineColor(sf::Color::Green);
		_saveText.setFillColor(sf::Color::Green);
	}

	_saveText.setFont(font);
	_saveText.setString("SAVE PLAN");
	_saveText.setCharacterSize(20);
	sf::FloatRect saveRect = _saveText.getLocalBounds();
	_saveText.setOrigin(saveRect.left + saveRect.width / 2.0f, saveRect.top + saveRect.height / 2.0f);
	_saveText.setPosition(startX + buttonWidth / 2.0f, saveY + buttonHeight / 2.0f);
}
void PlanState::InitDayColumns()
{
	_dayColumns.clear();

	sf::Font& font = this->_data->assets.getFont("Default Font");
	sf::Color headerColor(70, 130, 180);      // Steel Blue
	sf::Color buttonColor(50, 205, 50);       // Lime Green

	for (int day = 1; day <= 7; ++day)
	{
		DayColumn col;
		col.dayId = day;
		col.dayName = getDayShortName(day);

		float colX = _tableStartX + (day - 1) * _columnWidth;

		// --- NAG£ÓWEK ---
		col.headerBox.setSize(sf::Vector2f(_columnWidth - 4.0f, _headerHeight));
		col.headerBox.setPosition(colX + 2.0f, _tableStartY);
		col.headerBox.setFillColor(headerColor);
		col.headerBox.setOutlineThickness(1.0f);
		col.headerBox.setOutlineColor(sf::Color::White);

		col.headerText.setFont(font);
		col.headerText.setString(col.dayName);
		col.headerText.setCharacterSize(22);  // Wiêksza czcionka nag³ówka
		col.headerText.setFillColor(sf::Color::White);
		// Centrowanie tekstu w nag³ówku
		sf::FloatRect textBounds = col.headerText.getLocalBounds();
		col.headerText.setOrigin(textBounds.left + textBounds.width / 2.0f,
			textBounds.top + textBounds.height / 2.0f);
		col.headerText.setPosition(colX + _columnWidth / 2.0f, _tableStartY + _headerHeight / 2.0f);

		// --- PRZYCISK [+] ---
		float buttonWidth = _columnWidth - 20.0f;
		float buttonHeight = 40.0f;
		col.addButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
		col.addButton.setFillColor(sf::Color::Transparent);
		col.addButton.setOutlineThickness(2.0f);
		col.addButton.setOutlineColor(buttonColor);

		col.addButtonText.setFont(font);
		col.addButtonText.setString("+");
		col.addButtonText.setCharacterSize(32);  // Wiêkszy plus
		col.addButtonText.setFillColor(buttonColor);

		_dayColumns.push_back(col);
	}

	// Inicjalizacja scrollbara tabelki
	float scrollbarHeight = _tableMaxVisibleRows * _rowHeight;
	_tableScrollbar.setSize(sf::Vector2f(12.0f, scrollbarHeight));
	_tableScrollbar.setPosition(_tableStartX + 7 * _columnWidth + 10.0f, _tableStartY + _headerHeight + 10.0f);
	_tableScrollbar.setFillColor(sf::Color(60, 60, 60));

	_tableScrollThumb.setSize(sf::Vector2f(12.0f, 50.0f));
	_tableScrollThumb.setFillColor(sf::Color(120, 120, 120));
}

int PlanState::GetMaxTableRows()
{
	int maxRows = 0;
	for (const auto& col : _dayColumns) {
		if (static_cast<int>(col.entryIndices.size()) > maxRows) {
			maxRows = static_cast<int>(col.entryIndices.size());
		}
	}
	return maxRows;
}

void PlanState::UpdateTableScroll(int delta)
{
	int maxRows = GetMaxTableRows();
	int maxOffset = maxRows - _tableMaxVisibleRows;
	if (maxOffset < 0) maxOffset = 0;

	_tableScrollOffset += delta;
	if (_tableScrollOffset < 0) _tableScrollOffset = 0;
	if (_tableScrollOffset > maxOffset) _tableScrollOffset = maxOffset;

	// Aktualizuj pozycjê kciuka scrollbara
	float scrollbarHeight = _tableScrollbar.getSize().y;
	float thumbHeight = _tableScrollThumb.getSize().y;
	float scrollRange = scrollbarHeight - thumbHeight;
	float scrollRatio = (maxOffset > 0) ? (float)_tableScrollOffset / (float)maxOffset : 0.0f;

	_tableScrollThumb.setPosition(
		_tableScrollbar.getPosition().x,
		_tableScrollbar.getPosition().y + scrollRange * scrollRatio
	);

	// Odœwie¿ layout tabelki
	RefreshTableLayout();
}

void PlanState::RefreshTableLayout()
{
	// Najpierw przypisz æwiczenia do odpowiednich kolumn
	for (auto& col : _dayColumns) {
		col.entryIndices.clear();
	}

	for (size_t i = 0; i < _planEntries.size(); ++i) {
		int dayId = _planEntries[i]._dayId;
		if (dayId >= 1 && dayId <= 7) {
			_dayColumns[dayId - 1].entryIndices.push_back(static_cast<int>(i));
		}
	}

	// ZnajdŸ maksymaln¹ liczbê æwiczeñ w kolumnie
	int maxExercises = GetMaxTableRows();

	// Ustaw pozycje tekstów æwiczeñ i przycisków [+]
	sf::Font& font = this->_data->assets.getFont("Default Font");
	float contentStartY = _tableStartY + _headerHeight + 10.0f;

	for (int day = 0; day < 7; ++day)
	{
		auto& col = _dayColumns[day];
		float colX = _tableStartX + day * _columnWidth;
		float colCenterX = colX + _columnWidth / 2.0f;  // Œrodek kolumny

		// Ustaw pozycje tekstów æwiczeñ w tej kolumnie (z uwzglêdnieniem scrollowania)
		for (size_t j = 0; j < col.entryIndices.size(); ++j)
		{
			int entryIdx = col.entryIndices[j];
			int visualRow = static_cast<int>(j) - _tableScrollOffset;
			float posY = contentStartY + visualRow * _rowHeight;

			_planEntries[entryIdx]._nameText.setFont(font);
			_planEntries[entryIdx]._nameText.setString(_planEntries[entryIdx]._exerciseName);
			_planEntries[entryIdx]._nameText.setCharacterSize(18);
			_planEntries[entryIdx]._nameText.setFillColor(sf::Color::White);

			// Wycentruj tekst w kolumnie
			sf::FloatRect textBounds = _planEntries[entryIdx]._nameText.getLocalBounds();
			_planEntries[entryIdx]._nameText.setOrigin(
				textBounds.left + textBounds.width / 2.0f,
				textBounds.top + textBounds.height / 2.0f
			);
			_planEntries[entryIdx]._nameText.setPosition(colCenterX, posY + _rowHeight / 2.0f);
		}

		// Przycisk [+] na dole kolumny (po widocznych elementach)
		int visibleRows = min(maxExercises - _tableScrollOffset, _tableMaxVisibleRows);
		if (visibleRows < 0) visibleRows = 0;

		float buttonY = contentStartY + visibleRows * _rowHeight + 30.0f;
		float buttonWidth = _columnWidth - 20.0f;
		float buttonHeight = 40.0f;

		col.addButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
		col.addButton.setPosition(colX + 10.0f, buttonY);

		// Centruj tekst "+" w przycisku (dok³adnie na œrodku)
		col.addButtonText.setCharacterSize(32);
		sf::FloatRect txtBounds = col.addButtonText.getLocalBounds();
		col.addButtonText.setOrigin(
			txtBounds.left + txtBounds.width / 2.0f,
			txtBounds.top + txtBounds.height / 2.0f
		);
		col.addButtonText.setPosition(
			colX + 10.0f + buttonWidth / 2.0f,
			buttonY + buttonHeight / 2.0f
		);
	}

	// Aktualizuj rozmiar kciuka scrollbara tabelki
	if (maxExercises > _tableMaxVisibleRows)
	{
		float scrollbarHeight = _tableScrollbar.getSize().y;
		float visibleRatio = (float)_tableMaxVisibleRows / (float)maxExercises;
		float thumbHeight = scrollbarHeight * visibleRatio;
		if (thumbHeight < 30.0f) thumbHeight = 30.0f;

		_tableScrollThumb.setSize(sf::Vector2f(12.0f, thumbHeight));

		// Pozycja kciuka
		int maxOffset = maxExercises - _tableMaxVisibleRows;
		float scrollRange = scrollbarHeight - thumbHeight;
		float scrollRatio = (maxOffset > 0) ? (float)_tableScrollOffset / (float)maxOffset : 0.0f;
		_tableScrollThumb.setPosition(
			_tableScrollbar.getPosition().x,
			_tableScrollbar.getPosition().y + scrollRange * scrollRatio
		);
	}
}

void PlanState::InitPopup()
{
	sf::Font& font = this->_data->assets.getFont("Default Font");

	// T³o popup - mniejsze i wycentrowane
	_popupBackground.setSize(sf::Vector2f(350.0f, 450.0f));
	_popupBackground.setFillColor(sf::Color(40, 40, 40, 245));
	_popupBackground.setOutlineThickness(2.0f);
	_popupBackground.setOutlineColor(sf::Color::White);
	_popupBackground.setPosition(SCREAN_WIDTH / 2.0f - 175.0f, SCREAN_HEIGHT / 2.0f - 225.0f);

	// Tytu³
	_popupTitle.setFont(font);
	_popupTitle.setCharacterSize(22);
	_popupTitle.setFillColor(sf::Color::Yellow);
}

void PlanState::ShowAddExercisePopup(int dayId)
{
	_showAddPopup = true;
	_addPopupDayId = dayId;
	_popupScrollOffset = 0;  // Reset scrollowania

	// Pobierz wszystkie æwiczenia z bazy
	_allExercises = this->_data->database.getAllExercises();

	// Ustaw tytu³
	_popupTitle.setString("Add exercise - " + getDayShortName(dayId));
	sf::FloatRect titleBounds = _popupTitle.getLocalBounds();
	_popupTitle.setPosition(_popupBackground.getPosition().x + 175.0f - titleBounds.width / 2.0f,
		_popupBackground.getPosition().y + 10.0f);

	// Twórz teksty dla ka¿dego æwiczenia
	_popupExerciseTexts.clear();
	sf::Font& font = this->_data->assets.getFont("Default Font");

	float startY = _popupBackground.getPosition().y + 50.0f;
	float startX = _popupBackground.getPosition().x + 15.0f;
	float itemHeight = 32.0f;

	for (size_t i = 0; i < _allExercises.size(); ++i)
	{
		sf::Text txt;
		txt.setFont(font);
		txt.setString(_allExercises[i]->name);
		txt.setCharacterSize(16);
		txt.setFillColor(sf::Color::White);
		txt.setPosition(startX, startY + i * itemHeight);
		_popupExerciseTexts.push_back(txt);
	}

	// Konfiguracja scrollbara
	float scrollbarHeight = _popupBackground.getSize().y - 60.0f;
	_popupScrollbar.setSize(sf::Vector2f(10.0f, scrollbarHeight));
	_popupScrollbar.setPosition(_popupBackground.getPosition().x + _popupBackground.getSize().x - 15.0f,
		_popupBackground.getPosition().y + 50.0f);
	_popupScrollbar.setFillColor(sf::Color(60, 60, 60));

	// Rozmiar "kciuka" scrollbara zale¿y od liczby elementów
	float visibleRatio = (float)_popupMaxVisibleItems / (float)_allExercises.size();
	if (visibleRatio > 1.0f) visibleRatio = 1.0f;
	float thumbHeight = scrollbarHeight * visibleRatio;
	if (thumbHeight < 30.0f) thumbHeight = 30.0f;

	_popupScrollThumb.setSize(sf::Vector2f(10.0f, thumbHeight));
	_popupScrollThumb.setFillColor(sf::Color(120, 120, 120));
}

void PlanState::UpdatePopupScroll(int delta)
{
	int maxOffset = (int)_allExercises.size() - _popupMaxVisibleItems;
	if (maxOffset < 0) maxOffset = 0;

	_popupScrollOffset += delta;
	if (_popupScrollOffset < 0) _popupScrollOffset = 0;
	if (_popupScrollOffset > maxOffset) _popupScrollOffset = maxOffset;

	// Aktualizuj pozycjê "kciuka" scrollbara
	float scrollbarHeight = _popupScrollbar.getSize().y;
	float thumbHeight = _popupScrollThumb.getSize().y;
	float scrollRange = scrollbarHeight - thumbHeight;
	float scrollRatio = (maxOffset > 0) ? (float)_popupScrollOffset / (float)maxOffset : 0.0f;

	_popupScrollThumb.setPosition(
		_popupScrollbar.getPosition().x,
		_popupScrollbar.getPosition().y + scrollRange * scrollRatio
	);
}

void PlanState::HideAddExercisePopup()
{
	_showAddPopup = false;
	_addPopupDayId = -1;
	_popupExerciseTexts.clear();
	_popupScrollOffset = 0;
}

void PlanState::ShowError(const string& message)
{
	_errorMessage.setFont(this->_data->assets.getFont("Default Font"));
	_errorMessage.setString(message);
	_errorMessage.setCharacterSize(20);
	_errorMessage.setFillColor(sf::Color::Red);

	sf::FloatRect bounds = _errorMessage.getLocalBounds();
	_errorMessage.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	_errorMessage.setPosition(SCREAN_WIDTH / 2.0f, SCREAN_HEIGHT - 50.0f);

	_showError = true;
	_errorClock.restart();
}

// --- FUNKCJE DO TWORZENIA NOWEGO PLANU ---

void PlanState::InitNewPlanPopup()
{
	sf::Font& font = this->_data->assets.getFont("Default Font");

	_newPlanPopupBg.setSize(sf::Vector2f(400.0f, 200.0f));
	_newPlanPopupBg.setFillColor(sf::Color(40, 40, 40, 240));
	_newPlanPopupBg.setOutlineThickness(2.0f);
	_newPlanPopupBg.setOutlineColor(sf::Color::White);
	_newPlanPopupBg.setPosition(SCREAN_WIDTH / 2.0f - 200.0f, SCREAN_HEIGHT / 2.0f - 100.0f);

	_newPlanTitle.setFont(font);
	_newPlanTitle.setString("Create New Plan");
	_newPlanTitle.setCharacterSize(24);
	_newPlanTitle.setFillColor(sf::Color::Yellow);
	sf::FloatRect titleBounds = _newPlanTitle.getLocalBounds();
	_newPlanTitle.setPosition(_newPlanPopupBg.getPosition().x + 200.0f - titleBounds.width / 2.0f,
		_newPlanPopupBg.getPosition().y + 15.0f);

	_newPlanInputBox.setSize(sf::Vector2f(350.0f, 40.0f));
	_newPlanInputBox.setPosition(_newPlanPopupBg.getPosition().x + 25.0f,
		_newPlanPopupBg.getPosition().y + 60.0f);
	_newPlanInputBox.setFillColor(sf::Color(60, 60, 60));
	_newPlanInputBox.setOutlineThickness(1.0f);
	_newPlanInputBox.setOutlineColor(sf::Color::White);

	_newPlanInputText.setFont(font);
	_newPlanInputText.setCharacterSize(18);
	_newPlanInputText.setFillColor(sf::Color::White);
	_newPlanInputText.setPosition(_newPlanInputBox.getPosition().x + 10.0f,
		_newPlanInputBox.getPosition().y + 8.0f);

	_newPlanCreateButton.setSize(sf::Vector2f(150.0f, 40.0f));
	_newPlanCreateButton.setPosition(_newPlanPopupBg.getPosition().x + 125.0f,
		_newPlanPopupBg.getPosition().y + 130.0f);
	_newPlanCreateButton.setFillColor(sf::Color::Transparent);
	_newPlanCreateButton.setOutlineThickness(2.0f);
	_newPlanCreateButton.setOutlineColor(sf::Color::Green);

	_newPlanCreateText.setFont(font);
	_newPlanCreateText.setString("CREATE");
	_newPlanCreateText.setCharacterSize(20);
	_newPlanCreateText.setFillColor(sf::Color::Green);
	sf::FloatRect createBounds = _newPlanCreateText.getLocalBounds();
	_newPlanCreateText.setOrigin(createBounds.left + createBounds.width / 2.0f,
		createBounds.top + createBounds.height / 2.0f);
	_newPlanCreateText.setPosition(_newPlanCreateButton.getPosition().x + 75.0f,
		_newPlanCreateButton.getPosition().y + 20.0f);
}
void PlanState::ShowNewPlanPopup()
{
	_showNewPlanPopup = true;
	_newPlanName = "";
	_newPlanInputText.setString("");
}
void PlanState::HideNewPlanPopup()
{
	_showNewPlanPopup = false;
	_newPlanName = "";
}
void PlanState::CreateNewPlan()
{
	if (_newPlanName.empty()) {
		ShowError("Plan name cannot be empty!");
		return;
	}

	if (!this->_data->currentUser) {
		ShowError("You must be logged in!");
		return;
	}

	int userId = this->_data->currentUser->id;
	auto newPlan = this->_data->database.addTrainingPlan(_newPlanName, userId);

	if (newPlan) {
		this->_data->database.assignPlanToUser(userId, newPlan->id);
		this->_data->currentUser->currentPlanId = newPlan->id;

		HideNewPlanPopup();
		Plan_Init();
	}
	else {
		ShowError("Failed to create plan!");
	}
}

// --- FUNKCJE DO USUWANIA PLANU ---

bool PlanState::CanDeleteCurrentPlan()
{
	if (!this->_data->currentUser || !this->_data->currentUser->currentPlanId.has_value()) {
		return false;
	}

	int planId = this->_data->currentUser->currentPlanId.value();
	auto plan = this->_data->database.getTrainingPlanById(planId);

	if (!plan) return false;

	return plan->ownerId > 0;  // Tylko plany prywatne mo¿na usun¹æ
}
bool PlanState::CanEditCurrentPlan()
{
	if (!this->_data->currentUser || !this->_data->currentUser->currentPlanId.has_value()) {
		return false;
	}

	int planId = this->_data->currentUser->currentPlanId.value();
	auto plan = this->_data->database.getTrainingPlanById(planId);

	if (!plan) return false;

	// Mo¿na edytowaæ tylko plany prywatne (ownerId > 0)
	// Plany publiczne (ownerId == 0) s¹ tylko do odczytu
	return plan->ownerId > 0;
}
void PlanState::InitDeleteConfirmPopup()
{
	sf::Font& font = this->_data->assets.getFont("Default Font");

	_deleteConfirmBg.setSize(sf::Vector2f(450.0f, 180.0f));
	_deleteConfirmBg.setFillColor(sf::Color(50, 50, 50, 245));
	_deleteConfirmBg.setOutlineThickness(2.0f);
	_deleteConfirmBg.setOutlineColor(sf::Color::Red);
	_deleteConfirmBg.setPosition(SCREAN_WIDTH / 2.0f - 225.0f, SCREAN_HEIGHT / 2.0f - 90.0f);

	_deleteConfirmTitle.setFont(font);
	_deleteConfirmTitle.setString("DELETE PLAN?");
	_deleteConfirmTitle.setCharacterSize(26);
	_deleteConfirmTitle.setFillColor(sf::Color::Red);
	sf::FloatRect titleBounds = _deleteConfirmTitle.getLocalBounds();
	_deleteConfirmTitle.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top);
	_deleteConfirmTitle.setPosition(_deleteConfirmBg.getPosition().x + 225.0f,
		_deleteConfirmBg.getPosition().y + 15.0f);

	_deleteConfirmMessage.setFont(font);
	_deleteConfirmMessage.setString("Are you sure? This action cannot be undone.");
	_deleteConfirmMessage.setCharacterSize(18);
	_deleteConfirmMessage.setFillColor(sf::Color::White);
	sf::FloatRect msgBounds = _deleteConfirmMessage.getLocalBounds();
	_deleteConfirmMessage.setOrigin(msgBounds.left + msgBounds.width / 2.0f, msgBounds.top);
	_deleteConfirmMessage.setPosition(_deleteConfirmBg.getPosition().x + 225.0f,
		_deleteConfirmBg.getPosition().y + 60.0f);

	_deleteYesButton.setSize(sf::Vector2f(100.0f, 40.0f));
	_deleteYesButton.setPosition(_deleteConfirmBg.getPosition().x + 80.0f,
		_deleteConfirmBg.getPosition().y + 110.0f);
	_deleteYesButton.setFillColor(sf::Color::Transparent);
	_deleteYesButton.setOutlineThickness(2.0f);
	_deleteYesButton.setOutlineColor(sf::Color::Red);

	_deleteYesText.setFont(font);
	_deleteYesText.setString("YES");
	_deleteYesText.setCharacterSize(20);
	_deleteYesText.setFillColor(sf::Color::Red);
	sf::FloatRect yesBounds = _deleteYesText.getLocalBounds();
	_deleteYesText.setOrigin(yesBounds.left + yesBounds.width / 2.0f, yesBounds.top + yesBounds.height / 2.0f);
	_deleteYesText.setPosition(_deleteYesButton.getPosition().x + 50.0f,
		_deleteYesButton.getPosition().y + 20.0f);

	_deleteNoButton.setSize(sf::Vector2f(100.0f, 40.0f));
	_deleteNoButton.setPosition(_deleteConfirmBg.getPosition().x + 270.0f,
		_deleteConfirmBg.getPosition().y + 110.0f);
	_deleteNoButton.setFillColor(sf::Color::Transparent);
	_deleteNoButton.setOutlineThickness(2.0f);
	_deleteNoButton.setOutlineColor(sf::Color::Green);

	_deleteNoText.setFont(font);
	_deleteNoText.setString("NO");
	_deleteNoText.setCharacterSize(20);
	_deleteNoText.setFillColor(sf::Color::Green);
	sf::FloatRect noBounds = _deleteNoText.getLocalBounds();
	_deleteNoText.setOrigin(noBounds.left + noBounds.width / 2.0f, noBounds.top + noBounds.height / 2.0f);
	_deleteNoText.setPosition(_deleteNoButton.getPosition().x + 50.0f,
		_deleteNoButton.getPosition().y + 20.0f);
}
void PlanState::ShowDeleteConfirmPopup()
{
	if (!CanDeleteCurrentPlan()) {
		ShowError("Cannot delete public plans!");
		return;
	}
	_showDeleteConfirmPopup = true;
}
void PlanState::HideDeleteConfirmPopup()
{
	_showDeleteConfirmPopup = false;
}
void PlanState::DeleteCurrentPlan()
{
	if (!this->_data->currentUser || !this->_data->currentUser->currentPlanId.has_value()) {
		ShowError("No plan selected!");
		return;
	}

	int planId = this->_data->currentUser->currentPlanId.value();

	bool success = this->_data->database.deletePlan(planId);

	if (success) {

		this->_data->currentUser->currentPlanId = nullopt;

		auto availablePlans = this->_data->database.getPlansForUser(this->_data->currentUser->id);
		if (!availablePlans.empty()) {
			int newPlanId = availablePlans[0].id;
			this->_data->database.assignPlanToUser(this->_data->currentUser->id, newPlanId);
			this->_data->currentUser->currentPlanId = newPlanId;
		}

		HideDeleteConfirmPopup();
		_planListScrollOffset = 0;  // Reset scrollowania listy planów
		Plan_Init();
	}
	else {
		ShowError("Failed to delete plan!");
		HideDeleteConfirmPopup();
	}
}