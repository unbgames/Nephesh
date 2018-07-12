//
// Created by ftfnunes on 13/06/18.
//

#include <Collider.h>
#include <Sprite.h>
#include <InputManager.h>
#include <Boss.h>
#include "MeleeAttack.h"

#define PLAYER_MELEE_DAMAGE 10

MeleeAttack::MeleeAttack(GameObject &associated, string sprite, int frameCount, bool flip, Vec2 offset, Vec2 colScale, Vec2 colOffset, double attackDuration, bool debug) :
        Component(associated),
        collisionTimer(),
        colliderCreated(false),
        attackDuration(attackDuration),
        attackHit(false), flip(flip), offset(offset),
        colScale(colScale), colOffset(colOffset), debug(debug){
    
    if(sprite.size() > 0){
        Sprite *spr = new Sprite(associated, sprite, frameCount, attackDuration/frameCount, 0, false, flip);

        associated.AddComponent(spr);
        associated.box.h = spr->GetHeight();
        associated.box.w = spr->GetWidth();
        associated.SetCenter({associated.box.x, associated.box.y});

        associated.box += offset;
    }    
        
    auto collider = new Collider(associated, colScale, colOffset);
    collider->SetCanCollide([](GameObject& collidable) -> bool {
        return false;
    });
    associated.AddComponent(collider);
}

void MeleeAttack::Update(float dt) {
    if (debug) {
        auto &inputManager = InputManager::GetInstance();

        if (inputManager.IsKeyDown(UP_ARROW_KEY)) {
            associated.box.y -= 1;
        }
        if (inputManager.IsKeyDown(DOWN_ARROW_KEY)) {
            associated.box.y += 1;
        }
        if (inputManager.IsKeyDown(LEFT_ARROW_KEY)) {
            associated.box.x -= 1;
        }
        if (inputManager.IsKeyDown(RIGHT_ARROW_KEY)) {
            associated.box.x += 1;
        }

//        if (inputManager.KeyPress(SDLK_p)) {
//            auto playerCenter = Player::player->GetCenter();
//            auto center = associated.box.Center();
//            cout << center.x - playerCenter.x << ", " << center.y - playerCenter.y << endl;
//        }
    }
    durationTimer.Update(dt);

    if (durationTimer.Get() > attackDuration) {
        associated.RequestDelete();
    }
    auto created = colliderCreated;
    auto collider = (Collider *) associated.GetComponent(COLLIDER_TYPE);
    collider->SetCanCollide([created](GameObject& collidable) -> bool {
        return !created;
    });

    colliderCreated = true;
}

void MeleeAttack::Render() {

}

bool MeleeAttack::Is(string type) {
    return type == MELEE_ATTACK_TYPE;
}

void MeleeAttack::NotifyCollision(GameObject &other) {
    auto boss = (Boss *)other.GetComponent(BOSS_TYPE);
    if (boss != nullptr) {
        boss->DecreaseHp(PLAYER_MELEE_DAMAGE);
    }
    attackHit = true;
}

bool MeleeAttack::AttackHit() {
    return attackHit;
}
