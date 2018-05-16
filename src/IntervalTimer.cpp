//
// Created by ftfnunes on 09/05/18.
//

#include "IntervalTimer.h"

IntervalTimer::IntervalTimer(GameObject &associated, float timeLimit, function<void()> callback) : Component(associated),
                                                                                             timer(),
                                                                                             callback(callback),
                                                                                             timeLimit(timeLimit) { }

void IntervalTimer::Update(float dt) {
    timer.Update(dt);

    if (timer.Get() > timeLimit) {
        callback();
        timer.Restart();
    }
}

void IntervalTimer::Render() {}

bool IntervalTimer::Is(string type) {
    return false;
}
