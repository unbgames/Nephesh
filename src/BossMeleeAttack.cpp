//
// Created by fabio on 08/07/18.
//

#include "BossMeleeAttack.h"
#include <Collider.h>
#include <Sprite.h>
#include <InputManager.h>

BossMeleeAttack::BossMeleeAttack(GameObject &associated, string sprite, double attackDuration, bool flip, Vec2 offset, bool debug) :
        Component(associated),
        collisionTimer(),
        colliderCreated(false),
        attackDuration(attackDuration),
        attackHit(false), flip(flip), offset(offset), debug(debug){

    if(sprite.size() > 0){
        Sprite *spr = new Sprite(associated, sprite, 5, attackDuration/5, 0, false, flip);

        associated.AddComponent(spr);
        associated.box.h = spr->GetHeight();
        associated.box.w = spr->GetWidth();
        associated.SetCenter({associated.box.x, associated.box.y});

        associated.box += offset;
    }

    auto collider = new Collider(associated);
    collider->SetCanCollide([](GameObject& collidable) -> bool {
        return false;
    });
    associated.AddComponent(collider);
}

void BossMeleeAttack::Update(float dt) {
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

        if (inputManager.KeyPress(SDLK_p)) {
            auto playerCenter = Player::player->GetCenter();
            auto center = associated.box.Center();
            cout << center.x - playerCenter.x << ", " << center.y - playerCenter.y << endl;
        }
    }
    durationTimer.Update(dt);

    if (durationTimer.Get() > attackDuration) {
        associated.RequestDelete();
    }
//    auto created = colliderCreated;
//    auto collider = (Collider *) associated.GetComponent(COLLIDER_TYPE);
//    collider->SetCanCollide([created](GameObject& collidable) -> bool {
//        return !created;
//    });
//
//    colliderCreated = true;
}

void BossMeleeAttack::Render() {

}

bool BossMeleeAttack::Is(string type) {
    return type == BOSS_MELEE_ATTACK_TYPE;
}

void BossMeleeAttack::NotifyCollision(GameObject &other) {
    //other.RequestDelete();
    attackHit = true;
}

bool BossMeleeAttack::AttackHit() {
    return attackHit;
}
