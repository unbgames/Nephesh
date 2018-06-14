//
// Created by ftfnunes on 13/06/18.
//

#include <Player.h>
#include <Collider.h>
#include "MeleeAttack.h"

MeleeAttack::MeleeAttack(GameObject &associated) : Component(associated), collisionTimer(), colliderCreated(false) {
    auto collider = new Collider(associated);
    collider->SetCanCollide([](GameObject& collidable) -> bool {
        return false;
    });
    associated.AddComponent(collider);
}

void MeleeAttack::Update(float dt) {
    durationTimer.Update(dt);
    
    if (durationTimer.Get() > ATTACK_DURATION) {
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
    return false;
}

void MeleeAttack::NotifyCollision(GameObject &other) {
    other.RequestDelete();
}
