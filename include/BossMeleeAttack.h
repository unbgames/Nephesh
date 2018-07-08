//
// Created by fabio on 08/07/18.
//

#ifndef NEPHESH_BOSSMELEEATTACK_H
#define NEPHESH_BOSSMELEEATTACK_H

#define BOSS_MELEE_ATTACK_TYPE "BossMeleeAttack"

#include <Player.h>
#include "Component.h"
#include "Timer.h"


class BossMeleeAttack : public Component {
public:
    BossMeleeAttack(GameObject &associated, string sprite, int frameCount, double attackDuration = 
    PLAYER_ATTACK_DURATION, bool flip = false, Vec2 offset = {0, 0}, Vec2 colScale = {1,1}, Vec2 colOffset = {0,0}, bool debug = true);

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


#endif //NEPHESH_BOSSMELEEATTACK_H
