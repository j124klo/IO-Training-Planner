#include "DatabaseManager.hpp"

using namespace std;

void DatabaseManager::seedData()
{
    addUser("user1", "12345");

    addExercise("Push-ups", { ValueType::REPS });
    addExercise("Running", { ValueType::DISTANCE_KM, ValueType::TIME_SEC });
    addExercise("Deadlift", { ValueType::WEIGHT_KG, ValueType::REPS });
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

shared_ptr<User> DatabaseManager::getUserByLogin(const string& login) {
    for (auto& u : users) {
        if (u->login == login) return u;
    }
    return nullptr;
}

shared_ptr<Exercise> DatabaseManager::addExercise(const string& name, vector<ValueType> types) {
    auto ex = make_shared<Exercise>();
    ex->id = nextExId++;
    ex->name = name;
    ex->valueTypes = types;
    ex->isPublic = true;
    exercises.push_back(ex);
    return ex;
}

shared_ptr<Exercise> DatabaseManager::getExerciseById(int id) {
    for (auto& e : exercises) {
        if (e->id == id) return e;
    }
    return nullptr;
}

const vector<shared_ptr<Exercise>>& DatabaseManager::getAllExercises() const {
    return exercises;
}