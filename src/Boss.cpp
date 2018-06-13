//
// Created by bruno on 13/06/18.
//

#include "Boss.h"

Boss::Boss(GameObject &associated) :
        Component(associated),
        hp(BOSS_INITIAL_HP),
        speed(0,0),
        destination(0,0),
        bossState(IDLE),
        timer() {

}

void Boss::Update(float dt) {

}

void Boss::Render() {

}

bool Boss::Is(string type) {
    return type == BOSS_TYPE;
}
