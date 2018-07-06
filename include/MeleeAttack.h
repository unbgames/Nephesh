//
// Created by ftfnunes on 13/06/18.
//

#ifndef NEPHESH_MELEEATACK_H
#define NEPHESH_MELEEATACK_H


#include "Component.h"
#include "Timer.h"

#define MELEE_ATTACK_TYPE "MeleeAttack"

class MeleeAttack : public Component {
public:
    MeleeAttack(GameObject &associated);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void NotifyCollision(GameObject &other) override;

    bool AttackHit();

private:
    Timer collisionTimer;
    Timer durationTimer;
    bool colliderCreated;
    bool attackHit;
};


#endif //NEPHESH_MELEEATACK_H
