//
// Created by bruno on 13/06/18.
//

#include <Player.h>
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
    if(hp <= 0){
        associated.RequestDelete();

        auto explosionGO = new GameObject();
        auto explosionSound = new Sound(*explosionGO, "BOOM.wav");
        explosionGO->AddComponent(new Sprite(*explosionGO, "EXPLOSION.png", 4, 0.1, 0.4));
        explosionGO->AddComponent(explosionSound);
        explosionSound->Play();
        explosionGO->box.PlaceCenterAt(associated.box.Center());
        Game::GetInstance().GetCurrentState().AddObject(explosionGO);

        return;
    }

    if(Player::player){
        if(bossState == IDLE && timer.Get() < BOSS_IDLE_TIME){
            timer.Update(dt);
        }
        else if(bossState == IDLE){
            destination = Player::player->GetCenter();

            speed = {1, 0};
            speed = speed.Rotate(destination.DiferenceAngle(associated.box.Center()));

            bossState= MOVING;
        }
        else if(bossState == MOVING && destination.Distance(associated.box.Center()) <= BOSS_MIN_DIST_TO_PLAYER){

        }
        else if(bossState == ATTACKING && timer.Get() < BOSS_ATTACK_TIME){

        }
        else if(bossState == ATTACKING){

        }
    }

}

void Boss::Render() {

}

bool Boss::Is(string type) {
    return type == BOSS_TYPE;
}
