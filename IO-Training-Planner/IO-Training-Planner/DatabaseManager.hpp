#pragma once

#include <algorithm>
//#include <iostream>
#include <vector>
#include <memory>
#include <optional>
#include <sqlite_modern_cpp.h>
#include "Models.hpp"
#include "PlanEntry.hpp"

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

    // Users
    shared_ptr<User> addUser(const string& login, const string& pass);
    shared_ptr<User> getUserByLogin(const string& login);
    shared_ptr<User> getUserById(int id);
    bool assignPlanToUser(int userId, int planId);

    // Exercises
    shared_ptr<Exercise> addExercise(const string& name, const string& description, vector<ValueType> types);
    shared_ptr<Exercise> getExerciseById(int id);
    vector<shared_ptr<Exercise>> getAllExercises(); // Zmiana: zwracamy kopiê wektora, nie referencjê

    // Training Plans
    shared_ptr<TrainingPlan> addTrainingPlan(const string& name, int ownerId = 0);
    shared_ptr<TrainingPlan> getTrainingPlanById(int id);
    vector<PlanHeader> getAllPlanHeaders();
    vector<PlanHeader> getPlansForUser(int userId);  // NOWA: Pobiera plany dostêpne dla u¿ytkownika
    bool addExerciseToPlan(int planId, int exerciseId, int dayOfWeek, int orderInDay, std::vector<ResultValue> targets);
    bool updatePlanEntries(int planId, const vector<PlanGUIEntry>& newEntries);
    bool deletePlan(int planId);  // NOWA: Usuwa plan i jego wpisy

    // Workout Logs
    bool addWorkoutLog(int userId, int exerciseId, const string& date, int setNo, double weight, int reps);
    vector<WorkoutLog> getWorkoutLogs(int userId);

    // Personal Records
    vector<PersonalRecord> getBestWeights(int userId);
    vector<PersonalRecord> getBestReps(int userId);
    vector<PersonalRecord> getBestTimes(int userId);
    vector<PersonalRecord> getBestDistances(int userId);
};