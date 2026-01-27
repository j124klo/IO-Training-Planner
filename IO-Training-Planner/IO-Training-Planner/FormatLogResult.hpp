#pragma once
#include <string>
#include <sstream>
#include <memory>
#include "Models.hpp"

using namespace std;

inline string formatLogResult(const WorkoutLog& log, const shared_ptr<Exercise>& ex)
{
    if (!ex) {
        stringstream ss;
        ss << log.weight << " - " << log.reps;
        return ss.str();
    }

    bool hasDist = false;
    bool hasTime = false;
    for (auto vt : ex->valueTypes) {
        if (vt == ValueType::DISTANCE_KM) hasDist = true;
        if (vt == ValueType::TIME_SEC) hasTime = true;
    }

    stringstream ss;
    if (hasDist) {
        // Bieganie: km / min
        ss << log.weight << "km / " << log.reps << "min";
    }
    else if (hasTime) {
        // Plank: s
        ss << log.weight << "s";
    }
    else {
        // Si³ownia
        if (log.weight > 0) {
            ss << log.weight << "kg x " << log.reps;
        }
        else {
            // Jeœli ciê¿ar 0 (np. pompki), wyœwietl tylko powtórzenia
            ss << log.reps << " reps";
        }
    }

    return ss.str();
}