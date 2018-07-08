//
// Created by ftfnunes on 13/06/18.
//

#include <Collider.h>
#include "MeleeAttack.h"

MeleeAttack::MeleeAttack(GameObject &associated, double attackDuration) :
        Component(associated),
        collisionTimer(),
        colliderCreated(false),
        attackDuration(attackDuration),
        attackHit(false){
    auto collider = new Collider(associated);
    collider->SetCanCollide([](GameObject& collidable) -> bool {
        return false;
    });
    associated.AddComponent(collider);
}

void MeleeAttack::Update(float dt) {
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
//    other.RequestDelete();
    attackHit = true;
}

bool MeleeAttack::AttackHit() {
    return attackHit;
}
