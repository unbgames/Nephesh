//
// Created by bruno on 08/07/18.
//

#include "DecentTimer.h"

DecentTimer::DecentTimer(GameObject &associated, float timeLimit, function<void()> callback) :
        Component(associated),
        timer(),
        timeLimit(timeLimit),
        callback(callback){

}

void DecentTimer::Update(float dt) {
    timer.Update(dt);

    if (timer.Get() > timeLimit) {
        callback();
        associated.RemoveComponent(this);
    }
}

void DecentTimer::Render() {

}

bool DecentTimer::Is(string type) {
    return type == DECENT_TIMER_TYPE;
}

void DecentTimer::ProgramTimer(GameObject &associated, float timeLimit, function<void()> callback) {
    associated.AddComponent(new DecentTimer(associated, timeLimit, callback));
}
