#include "DatabaseManager.hpp"

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
}
