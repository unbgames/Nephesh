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

    auto currentTime = timer.Get();
    if (currentTime > timeLimit) {
        callback();
        timer.Restart(currentTime - timeLimit);
    }
}

void IntervalTimer::Render() {}

bool IntervalTimer::Is(string type) {
    return type == EVENT_TIMER_TYPE;
}
