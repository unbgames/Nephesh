//
// Created by fabio on 08/07/18.
//

#include "BossMeleeAttack.h"
#include <Collider.h>
#include <Sprite.h>
#include <InputManager.h>

BossMeleeAttack::BossMeleeAttack(GameObject &associated, int damage, string sprite, int frameCount, double 
attackDuration, bool flip, Vec2 offset, Vec2 colScale, Vec2 colOffset, bool debug) :
        Component(associated),
        collisionTimer(),
        colliderCreated(false),
        attackDuration(attackDuration),
        attackHit(false), flip(flip), offset(offset),
        colScale(colScale), damage(damage), colOffset(colOffset), debug(debug){

    if(sprite.size() > 0){
        Sprite *spr = new Sprite(associated, sprite, frameCount, attackDuration/frameCount, 0, false, flip);

        associated.AddComponent(spr);
        associated.box.h = spr->GetHeight();
        associated.box.w = spr->GetWidth();
        associated.SetCenter({associated.box.x, associated.box.y});

        associated.box += offset;
    }

    auto collider = new Collider(associated, colScale, colOffset);
    collider->SetCanCollide([] (GameObject &other) -> bool {
        return other.HasComponent(PLAYER_TYPE);
    });
    associated.AddComponent(collider);
}

void BossMeleeAttack::Update(float dt) {

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
    if(other.HasComponent(PLAYER_TYPE)){
        Player::player->DecreaseHp(damage);
        cout << "Hit player. Damage: " << damage << endl;
    }
    //other.RequestDelete();
    
}

bool BossMeleeAttack::AttackHit() {
    return attackHit;
}
