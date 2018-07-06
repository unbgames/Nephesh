//
// Created by ftfnunes on 06/07/18.
//

#include <Game.h>
#include "Charge.h"

Charge::Charge(GameObject &associated, GameObject *next, float duration) : Component(associated), next(next), chargeTimer(), duration(duration) {
}

void Charge::Update(float dt) {
    chargeTimer.Update(dt);
    if (chargeTimer.Get() > duration) {
        associated.RequestDelete();
        Game::GetInstance().GetCurrentState().AddObject(next);
    }
}

void Charge::Render() {

}

bool Charge::Is(string type) {
    return type == CHARGE_TYPE;
}
