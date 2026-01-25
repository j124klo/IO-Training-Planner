#pragma once

#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <optional>

using namespace std;


enum class ValueType { WEIGHT_KG, REPS, TIME_SEC, DISTANCE_KM };

using ResultValue = variant<int, float>;

struct Record {
    string date; // YYYY-MM-DD
    int exerciseId;
    vector<ResultValue> results;
};

struct Exercise {
    int id;
    string name;
    string description;
    bool isPublic;
    vector<int> allowedUserIds;
    vector<ValueType> valueTypes;
};

struct PlanEntry {
    int exerciseId;
    int dayOfWeek; // 0-6
    int orderInDay; // Kolejnoœæ æwiczenia w danym dniu (1, 2, 3, ...)
    vector<ResultValue> targetValues;
};

struct TrainingPlan {
    int id;
    string name;
    int ownerId = 0;  // 0 = publiczny plan, >0 = prywatny plan u¿ytkownika
    vector<PlanEntry> entries;
};

struct User {
    int id;
    string login;
    string password;
    vector<Record> history;
    optional<int> currentPlanId;
};

struct WorkoutLog {
    int id;
    int userId;
    int exerciseId;
    string date;        // YYYY-MM-DD
    int setNumber;
    double weight;
    int reps;

    string exerciseNameVal;
};


struct PersonalRecord {
    string exerciseName;
    double weight;  // G³ówny wynik (kg, sekundy lub km)
    int reps;       // Powtórzenia (dla cardio bêdzie 0)
    string date;
};

struct PlanHeader {
    int id;
    std::string name;
};