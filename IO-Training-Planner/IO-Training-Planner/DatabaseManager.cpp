#include "DatabaseManager.hpp"
#include <sstream> // Do budowania stringów (np. typy wartości)

using namespace std;
using namespace sqlite;

// Helper: zamienia wektor enumów na string (np. "0,1") do bazy
string serializeTypes(const vector<ValueType>& types) {
    stringstream ss;
    for (size_t i = 0; i < types.size(); ++i) {
        ss << static_cast<int>(types[i]);
        if (i < types.size() - 1) ss << ",";
    }
    return ss.str();
}

// Helper: odczytuje string z bazy do wektora
vector<ValueType> deserializeTypes(const string& s) {
    vector<ValueType> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, ',')) {
        if (!item.empty()) result.push_back(static_cast<ValueType>(stoi(item)));
    }
    return result;
}

DatabaseManager::DatabaseManager()
{
    try {
        // Tworzymy plik bazy danych
        db = make_unique<database>("training_planner.db");

        // Konfigurujemy tabele
        createTables();

        // Sprawdzamy czy są jacyś userzy, jak nie - dodajemy dane startowe
        //int count = 0;
        //*db << "SELECT count(*) FROM users;" >> count;
        //if (count == 0) {
        //    seedData();
        //}
        seedData();
    }
    catch (exception& e) {
        cout << "Database Init Error: " << e.what() << endl;
    }
}

void DatabaseManager::createTables() {
    // Tabela Users
    *db << "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "login TEXT UNIQUE, "
        "password TEXT, "
        "current_plan_id INTEGER DEFAULT 0"
        ");";

    // Tabela Exercises
    *db << "CREATE TABLE IF NOT EXISTS exercises ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT, "
        "description TEXT, "
        "value_types TEXT, " // Przechowujemy jako string "0,1"
        "is_public INTEGER DEFAULT 1"
        ");";

    // Tabela Plans
    *db << "CREATE TABLE IF NOT EXISTS plans ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT, "
        "owner_id INTEGER DEFAULT 0"  // 0 = publiczny, >0 = prywatny dla u�ytkownika
        ");";

    // Tabela PlanEntries (relacja plan <-> ćwiczenie)
    *db << "CREATE TABLE IF NOT EXISTS plan_entries ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "plan_id INTEGER, "
        "exercise_id INTEGER, "
        "day_of_week INTEGER, "
        "order_in_day INTEGER DEFAULT 1, "
        "target_val_1 REAL, "
        "target_val_2 REAL"
        ");";

    // Tabela WorkoutLogs
    *db << "CREATE TABLE IF NOT EXISTS workout_logs ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER, "
        "exercise_id INTEGER, "
        "date TEXT, "
        "set_number INTEGER, "
        "weight REAL, "
        "reps INTEGER"
        ");";
}

void DatabaseManager::seedData()
{
    cout << "Seeding database..." << endl;
    
    // Tworzymy tylko jednego użytkownika - admin
    auto admin = addUser("admin", "admin");

    auto exPushUps = addExercise("Push-ups", "Klasyczne pompki na klatke piersiowa", { ValueType::REPS });
    auto exRunning = addExercise("Running", "Bieganie w terenie lub na biezni", { ValueType::DISTANCE_KM, ValueType::TIME_SEC });
    auto exDeadlift = addExercise("Deadlift", "Martwy ciag ze sztanga", { ValueType::WEIGHT_KG, ValueType::REPS });

    // Nowe ćwiczenia
    auto exSquats = addExercise("Squats", "Przysiady ze sztanga", { ValueType::WEIGHT_KG, ValueType::REPS });
    auto exPlank = addExercise("Plank", "Deska - wzmacnianie brzucha", { ValueType::TIME_SEC });
    auto exCycling = addExercise("Cycling", "Jazda na rowerku-maszynie", { ValueType::DISTANCE_KM, ValueType::TIME_SEC });

    addExercise("Jumping Jacks", "Pajacyki - cwiczenie cardio", { ValueType::REPS });
    addExercise("Burpees", "Burpees - cwiczenie calego ciala", { ValueType::REPS });
    auto exBenchPress = addExercise("Bench Press", "Wyciskanie sztangi na lawce", { ValueType::WEIGHT_KG, ValueType::REPS });
    addExercise("Pull-ups", "Podciaganie na drazku", { ValueType::REPS });
    addExercise("Lunges", "Wykroki z hantlami", { ValueType::WEIGHT_KG, ValueType::REPS });
    addExercise("Sit-ups", "Brzuszki klasyczne", { ValueType::REPS });

    // --- PLAN 1: Full Body Workout ---
    string planName = "Full Body Workout";
    int existingPlanId = -1;

    try {
        *db << "SELECT id FROM plans WHERE name = ?;"
            << planName
            >> existingPlanId;
    }
    catch (...) {}

    if (existingPlanId != -1) {
        cout << "[INFO] Plan '" << planName << "' juz istnieje. Pomijam." << endl;
    }
    else {
        auto plan = addTrainingPlan(planName, 0);  // 0 = publiczny plan
        if (plan) {
            existingPlanId = plan->id;
            cout << "[INFO] Ukladanie harmonogramu dla: " << planName << endl;

            // PONIEDZIAŁEK (Dzień 1)
            if (exRunning)   addExerciseToPlan(plan->id, exRunning->id, 1, 1, { 2.0f, 15.0f });
            if (exDeadlift)  addExerciseToPlan(plan->id, exDeadlift->id, 1, 3, { 100.0f, 6 });
            if (exPushUps)   addExerciseToPlan(plan->id, exPushUps->id, 1, 2, { 10 });

            // ŚRODA (Dzień 3)
            if (exDeadlift)  addExerciseToPlan(plan->id, exDeadlift->id, 3, 2, { 100.0f, 5 });
            if (exSquats)    addExerciseToPlan(plan->id, exSquats->id, 3, 1, { 80.0f, 8 });

            // PIĄTEK (Dzień 5)
            if (exCycling)   addExerciseToPlan(plan->id, exCycling->id, 5, 1, { 15.0f, 45.0f });
            if (exPlank)     addExerciseToPlan(plan->id, exPlank->id, 5, 2, { 90.0f });
        }
    }

    // --- PLAN 2: Push Pull Legs ---
    string planName2 = "Push Pull Legs";
    int planId2 = -1;

    try { *db << "SELECT id FROM plans WHERE name = ?;" << planName2 >> planId2; }
    catch (...) {}

    if (planId2 == -1) {
        auto plan2 = addTrainingPlan(planName2, 0);  // 0 = publiczny plan
        if (plan2) {
            planId2 = plan2->id;
            
            // Push (Poniedziałek)
            if (exBenchPress) addExerciseToPlan(planId2, exBenchPress->id, 1, 1, { 80.0f, 8 });
            if (exPushUps)    addExerciseToPlan(planId2, exPushUps->id, 1, 2, { 15 });

            // Pull (Środa)
            if (exDeadlift)   addExerciseToPlan(planId2, exDeadlift->id, 3, 1, { 120.0f, 3 });

            // Legs (Piątek)
            if (exSquats)     addExerciseToPlan(planId2, exSquats->id, 5, 1, { 100.0f, 5 });

            cout << "[INFO] Utworzono drugi plan: " << planName2 << endl;
        }
    }

    // Przypisz pierwszy plan (Full Body Workout) do admina
    if (admin && existingPlanId != -1) {
        if (!admin->currentPlanId.has_value() || admin->currentPlanId.value() != existingPlanId) {
            assignPlanToUser(admin->id, existingPlanId);
            cout << "[SUCCESS] Przypisano plan '" << planName << "' do uzytkownika 'admin'." << endl;
        }
    }

    cout << "--- [SEED DATA] Done. ---" << endl;
}

/*shared_ptr<User> DatabaseManager::addUser(const string& login, const string& pass)
{
    try {
        *db << "INSERT INTO users (login, password) VALUES (?, ?);"
            << login << pass;

        int id = db->last_insert_rowid();
        return getUserById(id);
    }
    catch (exception& e) {
        cout << "Add User Error: " << e.what() << endl;
        return nullptr;
    }
}*/

shared_ptr<User> DatabaseManager::addUser(const string& login, const string& pass)
{
    // KROK 1: Sprawdź, czy użytkownik o takim loginie już istnieje
    // Wykorzystujemy Twoją gotową metodę pomocniczą
    auto existingUser = getUserByLogin(login);

    if (existingUser != nullptr) {
        // Użytkownik znaleziony!
        // Nie dodajemy duplikatu, po prostu zwracamy istniejącego.
        return existingUser;
    }

    // KROK 2: Jeśli nie znaleziono (existingUser == nullptr), to dodajemy nowego
    try {
        *db << "INSERT INTO users (login, password) VALUES (?, ?);"
            << login << pass; // Pamiętaj o haszowaniu, jeśli je już wdrożyłeś!

        int id = db->last_insert_rowid();

        cout << "[INFO] Zarejestrowano nowego uzytkownika: " << login << endl; // Log w konsoli
        return getUserById(id);
    }
    catch (exception& e) {
        cout << "Add User Error: " << e.what() << endl;
        return nullptr;
    }
}

shared_ptr<User> DatabaseManager::getUserByLogin(const string& login)
{
    auto user = make_shared<User>();
    bool found = false;

    // Pobieramy nullable int dla planu, bo w bazie może być NULL
    int id, planId = 0;
    string log, pass;

    try {
        *db << "SELECT id, login, password, IFNULL(current_plan_id, 0) FROM users WHERE login = ?;"
            << login
            >> [&](int _id, string _l, string _p, int _pid) {
            user->id = _id;
            user->login = _l;
            user->password = _p;
            if (_pid > 0) user->currentPlanId = _pid;
            found = true;
            };
    }
    catch (exception& e) {
        cout << "Get User Error: " << e.what() << endl;
    }

    return found ? user : nullptr;
}

shared_ptr<User> DatabaseManager::getUserById(int id)
{
    auto user = make_shared<User>();
    bool found = false;

    try {
        *db << "SELECT id, login, password, IFNULL(current_plan_id, 0) FROM users WHERE id = ?;"
            << id
            >> [&](int _id, string _l, string _p, int _pid) {
            user->id = _id;
            user->login = _l;
            user->password = _p;
            if (_pid > 0) user->currentPlanId = _pid;
            found = true;
            };
    }
    catch (...) {}

    return found ? user : nullptr;
}

bool DatabaseManager::assignPlanToUser(int userId, int planId)
{
    try {
        *db << "UPDATE users SET current_plan_id = ? WHERE id = ?;"
            << planId << userId;
        return true;
    }
    catch (exception& e) {
        return false;
    }
}

/*shared_ptr<Exercise> DatabaseManager::addExercise(const string& name, const string& description, vector<ValueType> types)
{
    string typesStr = serializeTypes(types);
    *db << "INSERT INTO exercises (name, description, value_types) VALUES (?, ?, ?);" << name << description << typesStr;

    int id = db->last_insert_rowid();
    return getExerciseById(id);
}*/

shared_ptr<Exercise> DatabaseManager::addExercise(const string& name, const string& description, vector<ValueType> types)
{
    // KROK 1: Sprawdzenie, czy ćwiczenie już istnieje
    int existingId = -1;

    try {
        // Próbujemy pobrać ID ćwiczenia o takiej samej nazwie
        *db << "SELECT id FROM exercises WHERE name = ?;"
            << name
            >> existingId;
    }
    catch (exception& e) {
        // Jeśli nie znaleziono, sqlite może rzucić wyjątek lub po prostu nie zmienić existingId
        // Ignorujemy to, bo -1 oznacza "nie znaleziono"
    }

    // KROK 2: Decyzja
    if (existingId != -1) {
        // Ćwiczenie już jest! Nie dublujemy go.
        // Zwracamy wskaźnik do tego, które już siedzi w bazie.
        // (Opcjonalnie: mógłbyś tu zrobić UPDATE, żeby zaktualizować opis, jeśli się zmienił)
        return getExerciseById(existingId);
    }

    // KROK 3: Dodawanie (tylko jeśli nie znaleziono)
    try {
        string typesStr = serializeTypes(types);
        *db << "INSERT INTO exercises (name, description, value_types) VALUES (?, ?, ?);"
            << name << description << typesStr;

        int newId = db->last_insert_rowid();
        cout << "[INFO] Dodano nowe cwiczenie: " << name << endl; // Log dla Twojej wiadomości
        return getExerciseById(newId);
    }
    catch (exception& e) {
        cout << "Add Exercise Error: " << e.what() << endl;
        return nullptr;
    }
}

shared_ptr<Exercise> DatabaseManager::getExerciseById(int id)
{
    auto ex = make_shared<Exercise>();
    bool found = false;
    string typeStr;

    *db << "SELECT id, name, description, value_types, is_public FROM exercises WHERE id = ?;"
        << id
        >> [&](int _id, string _n, string _desc, string _t, int _pub) {
        ex->id = _id;
        ex->name = _n;
        ex->description = _desc;
        ex->valueTypes = deserializeTypes(_t);
        ex->isPublic = (_pub != 0);
        found = true;
        };

    return found ? ex : nullptr;
}

vector<shared_ptr<Exercise>> DatabaseManager::getAllExercises()
{
    vector<shared_ptr<Exercise>> list;
    try {
        *db << "SELECT id, name, description, value_types, is_public FROM exercises;"
            >> [&](int _id, string _n, string _desc, string _t, int _pub) {
            auto ex = make_shared<Exercise>();
            ex->id = _id;
            ex->name = _n;
            ex->description = _desc;
            ex->valueTypes = deserializeTypes(_t);
            ex->isPublic = (_pub != 0);
            list.push_back(ex);
            };
    }
    catch (exception& e) {
        cout << "Get All Ex Error: " << e.what() << endl;
    }
    return list;
}

/* STARA WERSJA - USUNI�TA (u�ywamy wersji z ownerId)
shared_ptr<TrainingPlan> DatabaseManager::addTrainingPlan(const string& name)
{
    *db << "INSERT INTO plans (name) VALUES (?);" << name;
    int id = db->last_insert_rowid();
    return getTrainingPlanById(id);
}
*/

shared_ptr<TrainingPlan> DatabaseManager::getTrainingPlanById(int id)
{
    auto plan = make_shared<TrainingPlan>();
    bool found = false;

    // 1. Pobierz nagłówek planu z owner_id
    *db << "SELECT id, name, IFNULL(owner_id, 0) FROM plans WHERE id = ?;"
        << id
        >> [&](int _id, string _n, int _ownerId) {
        plan->id = _id;
        plan->name = _n;
        plan->ownerId = _ownerId;
        found = true;
        };

    if (!found) return nullptr;

    // 2. Pobierz wpisy (ćwiczenia w planie) SORTOWANE po order_in_day
    *db << "SELECT exercise_id, day_of_week, order_in_day, target_val_1, target_val_2 FROM plan_entries WHERE plan_id = ? ORDER BY day_of_week ASC, order_in_day ASC;"
        << id
        >> [&](int exId, int day, int orderInDay, double v1, double v2) {
        PlanEntry entry;
        entry.exerciseId = exId;
        entry.dayOfWeek = day;
        entry.orderInDay = orderInDay;
        if (v1 > 0) entry.targetValues.push_back((float)v1);
        if (v2 > 0) entry.targetValues.push_back((float)v2);
        plan->entries.push_back(entry);
        };

    return plan;
}

bool DatabaseManager::addExerciseToPlan(int planId, int exerciseId, int dayOfWeek, int orderInDay, std::vector<ResultValue> targets)
{
    // Uproszczenie: zapisujemy max 2 wartości celu do kolumn target_val_1 i 2
    // W pełnej wersji można by to robić w osobnej tabeli, ale na start wystarczy
    float v1 = 0, v2 = 0;

    // Wyciągamy wartości z varianta (zakładamy int lub float)
    if (targets.size() > 0) {
        if (holds_alternative<int>(targets[0])) v1 = get<int>(targets[0]);
        else v1 = get<float>(targets[0]);
    }
    if (targets.size() > 1) {
        if (holds_alternative<int>(targets[1])) v2 = get<int>(targets[1]);
        else v2 = get<float>(targets[1]);
    }

    try {
        *db << "INSERT INTO plan_entries (plan_id, exercise_id, day_of_week, order_in_day, target_val_1, target_val_2) VALUES (?, ?, ?, ?, ?, ?);"
            << planId << exerciseId << dayOfWeek << orderInDay << v1 << v2;
        return true;
    }
    catch (...) { return false; }
}

bool DatabaseManager::addWorkoutLog(int userId, int exerciseId, const string& date, int setNo, double weight, int reps)
{
    try {
        // Wstawiamy same liczby (ID) + dane treningowe
        *db << "INSERT INTO workout_logs (user_id, exercise_id, date, set_number, weight, reps) VALUES (?, ?, ?, ?, ?, ?);"
            << userId << exerciseId << date << setNo << weight << reps;
        return true;
    }
    catch (...) {
        return false;
    }
}

vector<WorkoutLog> DatabaseManager::getWorkoutLogs(int userId)
{
    vector<WorkoutLog> logs;
    try {
        // Pobieramy wpisy tylko dla ID tego konkretnego użytkownika
        // Dorzucamy (JOIN) tabelę exercises, żeby wiedzieć, jak nazywa się ćwiczenie o danym ID
        *db << "SELECT w.id, w.user_id, w.exercise_id, w.date, w.set_number, w.weight, w.reps, e.name "
            "FROM workout_logs w "
            "LEFT JOIN exercises e ON w.exercise_id = e.id "
            "WHERE w.user_id = ? "
            "ORDER BY w.date DESC, w.set_number ASC;"
            << userId
            >> [&](int _id, int _uid, int _eid, string _date, int _set, double _w, int _r, string _name) {

            WorkoutLog log;
            log.id = _id;
            log.userId = _uid;      // Twoje ID usera
            log.exerciseId = _eid;  // Twoje ID ćwiczenia
            log.date = _date;
            log.setNumber = _set;
            log.weight = _w;
            log.reps = _r;
            log.exerciseNameVal = _name; // Pomocnicza nazwa do wyświetlania w UI

            logs.push_back(log);
            };
    }
    catch (...) {}
    return logs;
}

// 1. STRENGTH (Siła) - Exercises without Time or Distance
vector<PersonalRecord> DatabaseManager::getBestWeights(int userId)
{
    vector<PersonalRecord> records;
    try {
        // Pobieramy ćwiczenia, które NIE są czasowe (2) ani dystansowe (3)
        *db << "SELECT e.name, MAX(w.weight), w.reps, w.date "
            "FROM workout_logs w "
            "JOIN exercises e ON w.exercise_id = e.id "
            "WHERE w.user_id = ? AND w.weight > 0 "
            "AND e.value_types NOT LIKE '%2%' AND e.value_types NOT LIKE '%3%' "
            "GROUP BY w.exercise_id "
            "ORDER BY MAX(w.weight) DESC;"
            << userId
            >> [&](string name, double w, int r, string date) {
            records.push_back({ name, w, r, date });
            };
    }
    catch (...) {}
    return records;
}

// 2. ENDURANCE (Wytrzymałość) - Max Reps
vector<PersonalRecord> DatabaseManager::getBestReps(int userId)
{
    vector<PersonalRecord> records;
    try {
        *db << "SELECT e.name, w.weight, MAX(w.reps), w.date "
            "FROM workout_logs w "
            "JOIN exercises e ON w.exercise_id = e.id "
            "WHERE w.user_id = ? AND w.reps > 0 "
            // --- POPRAWKA TUTAJ ---
            // Ignorujemy ćwiczenia, które są Czasowe (2) lub Dystansowe (3)
            // Dzięki temu "minuty" z biegania nie wpadną jako "powtórzenia"
            "AND e.value_types NOT LIKE '%2%' AND e.value_types NOT LIKE '%3%' "
            // ----------------------
            "GROUP BY w.exercise_id "
            "ORDER BY MAX(w.reps) DESC;"
            << userId
            >> [&](string name, double w, int r, string date) {
            records.push_back({ name, w, r, date });
            };
    }
    catch (...) {}
    return records;
}

/// 3. MAX TIME (Czas) - Typ 2 (TIME_SEC)
vector<PersonalRecord> DatabaseManager::getBestTimes(int userId)
{
    vector<PersonalRecord> records;
    try {
        *db << "SELECT e.name, MAX(w.weight), w.date "
            "FROM workout_logs w "
            "JOIN exercises e ON w.exercise_id = e.id "
            "WHERE w.user_id = ? "
            "AND e.value_types LIKE '%2%' " // Tylko typ CZAS
            "GROUP BY w.exercise_id "
            "ORDER BY MAX(w.weight) DESC;"
            << userId
            >> [&](string name, double w, string date) {
            records.push_back({ name, w, 0, date });
            };
    }
    catch (...) {}
    return records;
}

// 4. MAX DISTANCE (Dystans) - Typ 3 (DISTANCE_KM) - NOWA METODA
vector<PersonalRecord> DatabaseManager::getBestDistances(int userId)
{
    vector<PersonalRecord> records;
    try {
        *db << "SELECT e.name, MAX(w.weight), w.date "
            "FROM workout_logs w "
            "JOIN exercises e ON w.exercise_id = e.id "
            "WHERE w.user_id = ? "
            "AND e.value_types LIKE '%3%' " // Tylko typ DYSTANS
            "GROUP BY w.exercise_id "
            "ORDER BY MAX(w.weight) DESC;"
            << userId
            >> [&](string name, double w, string date) {
            // Tutaj w polu 'weight' przechowujemy kilometry
            records.push_back({ name, w, 0, date });
            };
    }
    catch (...) {}
    return records;
}

vector<PlanHeader> DatabaseManager::getAllPlanHeaders()
{
    vector<PlanHeader> headers;
    try {
        *db << "SELECT id, name FROM plans;"
            >> [&](int id, string name) {
            headers.push_back({ id, name });
            };
    }
    catch (exception& e) {
        cout << "Get All Plans Error: " << e.what() << endl;
    }
    return headers;
}

bool DatabaseManager::updatePlanEntries(int planId, const std::vector<PlanGUIEntry>& newEntries)
{
    try {
        *db << "BEGIN TRANSACTION;";

        // 1. Wyczyść stare wpisy dla tego planu
        *db << "DELETE FROM plan_entries WHERE plan_id = ?;" << planId;

        // 2. Wstaw nowe wpisy (zaktualizowane)
        // Musimy przeliczyć order_in_day, żeby zachować kolejność z GUI

        // Grupujemy liczniki order_in_day dla każdego dnia (1->1, 1->2, 3->1 etc.)
        std::map<int, int> dayCounters;

        for (const auto& entry : newEntries) {
            int currentDay = entry._dayId;
            int order = ++dayCounters[currentDay]; // Zwiększ licznik dla tego dnia

            float v1 = 0.0f, v2 = 0.0f;
            if (entry._targets.size() > 0) {
                if (std::holds_alternative<int>(entry._targets[0])) v1 = (float)std::get<int>(entry._targets[0]);
                else v1 = std::get<float>(entry._targets[0]);
            }
            if (entry._targets.size() > 1) {
                if (std::holds_alternative<int>(entry._targets[1])) v2 = (float)std::get<int>(entry._targets[1]);
                else v2 = std::get<float>(entry._targets[1]);
            }

            *db << "INSERT INTO plan_entries (plan_id, exercise_id, day_of_week, order_in_day, target_val_1, target_val_2) VALUES (?, ?, ?, ?, ?, ?);"
                << planId
                << entry._exerciseId
                << currentDay
                << order      // Nowy, przeliczony order_in_day
                << v1 << v2;
        }

        *db << "COMMIT;";
        cout << "[DB] Zapisano " << newEntries.size() << " wpisow do planu ID " << planId << endl;
        return true;
    }
    catch (std::exception& e) {
        std::cout << "Update Plan Error: " << e.what() << std::endl;
        *db << "ROLLBACK;"; // Cofnij zmiany w razie błędu
        return false;
    }
}

shared_ptr<TrainingPlan> DatabaseManager::addTrainingPlan(const string& name, int ownerId)
{
    *db << "INSERT INTO plans (name, owner_id) VALUES (?, ?);" << name << ownerId;
    int id = db->last_insert_rowid();
    return getTrainingPlanById(id);
}

vector<PlanHeader> DatabaseManager::getPlansForUser(int userId)
{
    vector<PlanHeader> headers;
    try {
        // Pobierz plany publiczne (owner_id = 0) ORAZ prywatne plany tego u�ytkownika
        *db << "SELECT id, name FROM plans WHERE owner_id = 0 OR owner_id = ?;"
            << userId
            >> [&](int id, string name) {
            headers.push_back({ id, name });
            };
    }
    catch (exception& e) {
        cout << "Get Plans For User Error: " << e.what() << endl;
    }
    return headers;
}

bool DatabaseManager::deletePlan(int planId)
{
    try {
        // Sprawdź czy plan istnieje i czy nie jest publiczny
        int ownerId = 0;
        *db << "SELECT owner_id FROM plans WHERE id = ?;" << planId >> ownerId;
        
        if (ownerId == 0) {
            cout << "[ERROR] Cannot delete public plan (ID: " << planId << ")" << endl;
            return false;  // Nie można usunąć planu publicznego
        }
        
        *db << "BEGIN TRANSACTION;";
        
        // 1. Usuń wszystkie wpisy planu
        *db << "DELETE FROM plan_entries WHERE plan_id = ?;" << planId;
        
        // 2. Usuń sam plan
        *db << "DELETE FROM plans WHERE id = ?;" << planId;
        
        // 3. Wyczyść przypisanie planu u użytkowników którzy go mieli
        *db << "UPDATE users SET current_plan_id = 0 WHERE current_plan_id = ?;" << planId;
        
        *db << "COMMIT;";
        
        cout << "[SUCCESS] Plan ID " << planId << " zostal usuniety." << endl;
        return true;
    }
    catch (exception& e) {
        cout << "[ERROR] Delete Plan Error: " << e.what() << endl;
        *db << "ROLLBACK;";
        return false;
    }
}