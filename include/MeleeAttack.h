//
// Created by ftfnunes on 13/06/18.
//

#ifndef NEPHESH_MELEEATACK_H
#define NEPHESH_MELEEATACK_H

#include <Player.h>
#include "Component.h"
#include "Timer.h"

#define MELEE_ATTACK_TYPE "MeleeAttack"

class MeleeAttack : public Component {
public:
    MeleeAttack(GameObject &associated, string sprite, int frameCount, bool flip = false, Vec2 offset = {0, 0}, Vec2 colScale = {1,1}, Vec2 colOffset = {0,0}, double attackDuration = PLAYER_ATTACK_DURATION, bool debug = 
    true);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void NotifyCollision(GameObject &other) override;

    bool AttackHit();

private:
    Timer collisionTimer;
    Timer durationTimer;
    bool colliderCreated;
    double attackDuration;
    bool attackHit;
    bool debug;
    bool flip;
    Vec2 offset;
    Vec2 colScale;
    Vec2 colOffset;
};


#endif //NEPHESH_MELEEATACK_H
