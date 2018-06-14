//
// Created by bruno on 13/06/18.
//

#ifndef NEPHESH_BOSS_H
#define NEPHESH_BOSS_H

#include "Component.h"
#include "Vec2.h"
#include "Timer.h"
#include <Sound.h>
#include <Sprite.h>
#include <Game.h>

#define BOSS_TYPE "Boss"
#define BOSS_INITIAL_HP 100
#define BOSS_IDLE_TIME 3 // in seconds
#define BOSS_ATTACK_TIME 3 // in seconds
#define BOSS_SPEED 300 // in pixels/s
#define BOSS_MIN_DIST_TO_PLAYER 200 // in pixels
#define BOSS_SPR_MOV_TIME 0.05
#define BOSS_SPR_ATCK_TIME 0.2

using namespace std;

class Boss : public Component {
public:
    explicit Boss(GameObject& associated);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

private:
    enum BossState {
        ATTACKING,
        MOVING,
        IDLE
    };
    
    enum BossDirection{
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    int hp;
    Vec2 speed;
    Vec2 destination;
    BossState bossState;
    Timer timer;
    
    BossDirection currentDirection;
    BossDirection GetNewDirection();
    vector<pair<BossDirection, string>> movementAnimations;
    vector<pair<BossDirection, string>> attackAnimations;

    string GetMovementAnimation();
    string GetAttackAnimation();
    void SetSprite(string file, int frameCount, float frameTime, bool flip = false);

};


#endif //NEPHESH_BOSS_H
