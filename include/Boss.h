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
#define BOSS_SPEED 300 // in pixels/s
#define BOSS_MIN_DIST_TO_PLAYER 20 // in pixels
#define BOSS_SPR_MOV_TIME 0.05 // in seconds
#define BOSS_ATTACK_TIME 1.0 // in seconds
#define BOSS_NUM_OF_ATTACKS 3
#define BOSS_ATTACK_SPRITE_COUNT 4
#define BOSS_ATTACK_RANGE 100 // in pixels
#define BOSS_ATTACK_WIDTH 200 // in pixels

using namespace std;

class Boss : public Component {
public:
    explicit Boss(GameObject& associated);

    void Update(float dt) override;
    void Render() override;
    bool Is(string type) override;

    void Attack();

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
    BossState bossState;
    Timer timer;
    int attacksPerformed;
    
    BossDirection currentDirection;
    BossDirection GetNewDirection();
    vector<pair<BossDirection, string>> movementAnimations;
    vector<pair<BossDirection, string>> attackAnimations;

    string GetMovementAnimation();
    string GetAttackAnimation();
    void SetSprite(string file, int frameCount, float frameTime, bool flip = false);

};


#endif //NEPHESH_BOSS_H
