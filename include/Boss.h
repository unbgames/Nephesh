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
#include <CameraShaker.h>

#define BOSS_TYPE "Boss"
#define BOSS_INITIAL_HP 100
#define BOSS_IDLE_TIME 3 // in seconds
#define BOSS_ATTACK_TIME 1.0 // in seconds
#define BOSS_MIN_NUM_OF_ATTACKS 1
#define BOSS_MAX_NUM_OF_ATTACKS 5
#define BOSS_SLAP_DISTANCE 250

#define BOSS_IDLE_SPRITE "img/boss_idle.png"
#define BOSS_SLAP_RIGHT_SPRITE "img/boss_slap_right.png"
#define BOSS_SLAP_LEFT_SPRITE "img/boss_slap_left.png"
#define BOSS_SLAM_SPRITE "img/boss_slam.png"
#define BOSS_CLAP_SPRITE "img/boss_clap.png"


using namespace std;

class Boss : public Component {
public:
    enum BossState {
        ATTACKING,
        IDLE
    };

    enum BossAttack{
        SLAP = 0,
        SLAM = 1,
        CLAP = 2
    };

    explicit Boss(GameObject& associated);

    void Update(float dt) override;
    void Render() override;
    bool Is(string type) override;

    void Attack();
    void UpdateState(BossState newState);

    void RockSlide();

    void SlapAttack();
    void SlamAttack();
    void ClapAttack();

private:

    int hp;
    Vec2 speed;
    BossState currentState;
    BossState previousState;
    BossAttack attackState;
    Timer timer;
    int attacksPerformed;
    int numOfAttacks;
    CameraShaker* camShaker;
    
    void SetSprite(string file, bool flip = false);

    void PrintBossState();
    void PrintBossAttack();

    void PlaySound(string file);

};


#endif //NEPHESH_BOSS_H
