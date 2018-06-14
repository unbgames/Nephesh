//
// Created by ftfnunes on 13/06/18.
//

#ifndef NEPHESH_MELEEATACK_H
#define NEPHESH_MELEEATACK_H

#include <Player.h>
#include "Component.h"
#include "Timer.h"

class MeleeAttack : public Component {
public:
    MeleeAttack(GameObject &associated, double attackDuration = ATTACK_DURATION);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void NotifyCollision(GameObject &other) override;

private:
    Timer collisionTimer;
    Timer durationTimer;
    bool colliderCreated;
    double attackDuration;
};


#endif //NEPHESH_MELEEATACK_H
