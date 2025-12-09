#pragma once

#include <algorithm>
#include <iostream>

#include "Models.hpp"

using namespace std;

class DatabaseManager {
private:

    vector<shared_ptr<User>> users;
    vector<shared_ptr<Exercise>> exercises;
    vector<shared_ptr<TrainingPlan>> plans;

	//  ID counters (klucze g³ówne w sql)
    int nextUserId = 1;
    int nextExId = 1;
    int nextPlanId = 1;

    void seedData();

public:
    DatabaseManager();

    shared_ptr<User> addUser(const string& login, const string& pass);

    shared_ptr<User> getUserByLogin(const string& login);

    shared_ptr<Exercise> addExercise(const string& name, vector<ValueType> types);

    shared_ptr<Exercise> getExerciseById(int id);

    const vector<shared_ptr<Exercise>>& getAllExercises() const;

};