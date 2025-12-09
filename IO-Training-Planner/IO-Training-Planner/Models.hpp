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
    vector<ResultValue> targetValues;
};

struct TrainingPlan {
    int id;
    string name;
    vector<PlanEntry> entries;
};

struct User {
    int id;
    string login;
    string password;
    vector<Record> history;
    optional<int> currentPlanId;
};