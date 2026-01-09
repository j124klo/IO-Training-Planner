/*#pragma once

#include <algorithm>
#include <iostream>

#include "Models.hpp"

using namespace std;

class DatabaseManager {
private:

    vector<shared_ptr<User>> users;
    vector<shared_ptr<Exercise>> exercises;
    vector<shared_ptr<TrainingPlan>> plans;

    int nextUserId = 1;
    int nextExId = 1;
    int nextPlanId = 1;

    void seedData();

public:
    DatabaseManager();

    shared_ptr<User> addUser(const string& login, const string& pass);
    shared_ptr<User> getUserByLogin(const string& login);
	shared_ptr<User> getUserById(int id);
    bool assignPlanToUser(int userId, int planId);

    shared_ptr<Exercise> addExercise(const string& name, vector<ValueType> types);
    shared_ptr<Exercise> getExerciseById(int id);
    const vector<shared_ptr<Exercise>>& getAllExercises() const;

    shared_ptr<TrainingPlan> addTrainingPlan(const string& name);
	shared_ptr<TrainingPlan> getTrainingPlanById(int id);
    bool addExerciseToPlan(int planId, int exerciseId, int dayOfWeek, std::vector<ResultValue> targets);
    
};*/

#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>
#include <optional>
#include <sqlite_modern_cpp.h>

#include "Models.hpp"

using namespace std;

class DatabaseManager {
private:
    // To jest nasza baza danych.
    // unique_ptr pozwala opóŸniæ jej inicjalizacjê do konstruktora
    unique_ptr<sqlite::database> db;

    void seedData();
    void createTables(); // Nowa metoda do tworzenia tabel

public:
    DatabaseManager();

    // Metody pozostaj¹ bez zmian w nag³ówkach (API jest to samo)
    shared_ptr<User> addUser(const string& login, const string& pass);
    shared_ptr<User> getUserByLogin(const string& login);
    shared_ptr<User> getUserById(int id);
    bool assignPlanToUser(int userId, int planId);

    shared_ptr<Exercise> addExercise(const string& name, vector<ValueType> types);
    shared_ptr<Exercise> getExerciseById(int id);
    vector<shared_ptr<Exercise>> getAllExercises(); // Zmiana: zwracamy kopiê wektora, nie referencjê

    shared_ptr<TrainingPlan> addTrainingPlan(const string& name);
    shared_ptr<TrainingPlan> getTrainingPlanById(int id);
    bool addExerciseToPlan(int planId, int exerciseId, int dayOfWeek, std::vector<ResultValue> targets);
};