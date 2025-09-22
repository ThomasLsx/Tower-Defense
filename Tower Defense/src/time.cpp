#include "time.h"

Time::Time() : deltaTime(0.0f), elapsed(0.0f) {}

void Time::tick(float dt) {
    deltaTime = dt;
    elapsed += dt;
}

float Time::getDelta() const {
    return deltaTime;
}

float Time::getElapsed() const {
    return elapsed;
}

void Time::reset() {
    elapsed = 0.0f;
    deltaTime = 0.0f;
}
