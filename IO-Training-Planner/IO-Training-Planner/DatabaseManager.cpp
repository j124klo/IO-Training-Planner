/*#include "DatabaseManager.hpp"

using namespace std;

void DatabaseManager::seedData()
{
    addUser("a", "a");

    addExercise("Push-ups", { ValueType::REPS });
    addExercise("Running", { ValueType::DISTANCE_KM, ValueType::TIME_SEC });
    addExercise("Deadlift", { ValueType::WEIGHT_KG, ValueType::REPS });

	addTrainingPlan("Beginner Plan");
	addExerciseToPlan(1, 1, 1, { 20 });
    addExerciseToPlan(1, 2, 3, { 9.5f, 3600.0f });
	addExerciseToPlan(1, 3, 5, { 100.0f, 5 });
	assignPlanToUser(1, 1);
}

DatabaseManager::DatabaseManager()
{
    seedData();
}

shared_ptr<User> DatabaseManager::addUser(const string& login, const string& pass)
{
    auto newUser = make_shared<User>();
    newUser->id = nextUserId++;
    newUser->login = login;
    newUser->password = pass;
    users.push_back(newUser);
    return newUser;
}

shared_ptr<User> DatabaseManager::getUserByLogin(const string& login)
{
    for (auto& u : users) {
        if (u->login == login) return u;
    }
    return nullptr;
}

shared_ptr<User> DatabaseManager::getUserById(int id)
{
    for (auto& u : users) {
        if (u->id == id) return u;
    }
    return nullptr;
}

bool DatabaseManager::assignPlanToUser(int userId, int planId)
{
    for (auto& u : users) {
        if (u->id == userId) {
            // SprawdŸ czy plan istnieje
            if (getTrainingPlanById(planId)) {
                u->currentPlanId = planId;
                return true;
            }
        }
    }
    return false;
}


shared_ptr<Exercise> DatabaseManager::addExercise(const string& name, vector<ValueType> types)
{
    auto ex = make_shared<Exercise>();
    ex->id = nextExId++;
    ex->name = name;
    ex->valueTypes = types;
    ex->isPublic = true;
    exercises.push_back(ex);
    return ex;
}

shared_ptr<Exercise> DatabaseManager::getExerciseById(int id)
{
    for (auto& e : exercises) {
        if (e->id == id) return e;
    }
    return nullptr;
}

const vector<shared_ptr<Exercise>>& DatabaseManager::getAllExercises() const
{
    return exercises;
}


shared_ptr<TrainingPlan> DatabaseManager::addTrainingPlan(const string& name)
{
    auto plan = make_shared<TrainingPlan>();
    plan->id = nextPlanId++;
    plan->name = name;
    plans.push_back(plan);
    return plan;
}

shared_ptr<TrainingPlan> DatabaseManager::getTrainingPlanById(int id)
{
    for (auto& p : plans) {
        if (p->id == id) return p;
    }
    return nullptr;
}

bool DatabaseManager::addExerciseToPlan(int planId, int exerciseId, int dayOfWeek, vector<ResultValue> targets)
{
    auto plan = getTrainingPlanById(planId);
    auto exercise = getExerciseById(exerciseId);

    if (plan && exercise) {
        PlanEntry entry;
        entry.exerciseId = exerciseId;
        entry.dayOfWeek = dayOfWeek;
        entry.targetValues = targets;

        plan->entries.push_back(entry);
        return true;
    }
    return false;
}*/

#include "DatabaseManager.hpp"
#include <sstream> // Do budowania stringów (np. typy wartoœci)

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

        // Sprawdzamy czy s¹ jacyœ userzy, jak nie - dodajemy dane startowe
        int count = 0;
        *db << "SELECT count(*) FROM users;" >> count;
        if (count == 0) {
            seedData();
        }
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
        "name TEXT"
        ");";

    // Tabela PlanEntries (relacja plan <-> æwiczenie)
    *db << "CREATE TABLE IF NOT EXISTS plan_entries ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "plan_id INTEGER, "
        "exercise_id INTEGER, "
        "day_of_week INTEGER, "
        "target_val_1 REAL, " // Uproszczenie: trzymamy 2 wartoœci celu
        "target_val_2 REAL"
        ");";
}

void DatabaseManager::seedData()
{
    cout << "Seeding database..." << endl;
    addUser("a", "a"); // Twoje "a", "a" zmieni³em na admin/admin
    addUser("b", "b");

    addExercise("Push-ups", "Klasyczne pompki na klatke piersiowa", { ValueType::REPS });
    addExercise("Running", "Bieganie w terenie lub na biezni", { ValueType::DISTANCE_KM, ValueType::TIME_SEC });
    addExercise("Deadlift", "Martwy ciag ze sztanga", { ValueType::WEIGHT_KG, ValueType::REPS });

    auto plan = addTrainingPlan("Beginner Plan");
    // Przyk³adowe przypisanie (zak³adamy ID 1, 2, 3 bo dopiero dodaliœmy)
    addExerciseToPlan(plan->id, 1, 1, { 20 });
    assignPlanToUser(1, plan->id);
}

shared_ptr<User> DatabaseManager::addUser(const string& login, const string& pass)
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
}

shared_ptr<User> DatabaseManager::getUserByLogin(const string& login)
{
    auto user = make_shared<User>();
    bool found = false;

    // Pobieramy nullable int dla planu, bo w bazie mo¿e byæ NULL
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

shared_ptr<Exercise> DatabaseManager::addExercise(const string& name, const string& description, vector<ValueType> types)
{
    string typesStr = serializeTypes(types);
    *db << "INSERT INTO exercises (name, description, value_types) VALUES (?, ?, ?);" << name << description << typesStr;

    int id = db->last_insert_rowid();
    return getExerciseById(id);
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

shared_ptr<TrainingPlan> DatabaseManager::addTrainingPlan(const string& name)
{
    *db << "INSERT INTO plans (name) VALUES (?);" << name;
    int id = db->last_insert_rowid();
    return getTrainingPlanById(id);
}

shared_ptr<TrainingPlan> DatabaseManager::getTrainingPlanById(int id)
{
    auto plan = make_shared<TrainingPlan>();
    bool found = false;

    // 1. Pobierz nag³ówek planu
    *db << "SELECT id, name FROM plans WHERE id = ?;"
        << id
        >> [&](int _id, string _n) {
        plan->id = _id;
        plan->name = _n;
        found = true;
        };

    if (!found) return nullptr;

    // 2. Pobierz wpisy (æwiczenia w planie)
    *db << "SELECT exercise_id, day_of_week, target_val_1, target_val_2 FROM plan_entries WHERE plan_id = ?;"
        << id
        >> [&](int exId, int day, double v1, double v2) {
        PlanEntry entry;
        entry.exerciseId = exId;
        entry.dayOfWeek = day;
        // Tutaj proste mapowanie - mo¿na rozbudowaæ w zale¿noœci od typu æwiczenia
        if (v1 > 0) entry.targetValues.push_back((float)v1);
        if (v2 > 0) entry.targetValues.push_back((float)v2);
        plan->entries.push_back(entry);
        };

    return plan;
}

bool DatabaseManager::addExerciseToPlan(int planId, int exerciseId, int dayOfWeek, std::vector<ResultValue> targets)
{
    // Uproszczenie: zapisujemy max 2 wartoœci celu do kolumn target_val_1 i 2
    // W pe³nej wersji mo¿na by to robiæ w osobnej tabeli, ale na start wystarczy
    float v1 = 0, v2 = 0;

    // Wyci¹gamy wartoœci z varianta (zak³adamy int lub float)
    if (targets.size() > 0) {
        if (holds_alternative<int>(targets[0])) v1 = get<int>(targets[0]);
        else v1 = get<float>(targets[0]);
    }
    if (targets.size() > 1) {
        if (holds_alternative<int>(targets[1])) v2 = get<int>(targets[1]);
        else v2 = get<float>(targets[1]);
    }

    try {
        *db << "INSERT INTO plan_entries (plan_id, exercise_id, day_of_week, target_val_1, target_val_2) VALUES (?, ?, ?, ?, ?);"
            << planId << exerciseId << dayOfWeek << v1 << v2;
        return true;
    }
    catch (...) { return false; }
}