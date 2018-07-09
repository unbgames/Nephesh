//
// Created by ftfnunes on 06/07/18.
//

#include <Game.h>
#include "Cast.h"

Cast::Cast(GameObject &associated, GameObject *next, float duration) : Component(associated), next(next), castTimer(), duration(duration) {
}

void Cast::Update(float dt) {
    castTimer.Update(dt);
    if (castTimer.Get() > duration) {
        associated.RequestDelete();
        if(next != nullptr){
            Game::GetInstance().GetCurrentState().AddObject(next);
        }
        
    }
}

void Cast::Render() {

}

bool Cast::Is(string type) {
    return type == CAST_TYPE;
}
