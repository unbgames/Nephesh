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
#define BOSS_IDLE_TIME 4 // in seconds
#define BOSS_SPEED 300 // in pixels/s
#define BOSS_MIN_DIST_TO_PLAYER 100 // in pixels
#define BOSS_SPR_MOV_TIME 0.05 // in seconds
#define BOSS_ATTACK_TIME 1.0 // in seconds
#define BOSS_MIN_NUM_OF_ATTACKS 1
#define BOSS_MAX_NUM_OF_ATTACKS 1
#define BOSS_ATTACK_SPRITE_COUNT 4
#define BOSS_ATTACK_RANGE 100 // in pixels
#define BOSS_ATTACK_WIDTH 200 // in pixels
#define BOSS_SLAP_DISTANCE 400
#define BOSS_IDLE_DISTANCE 1000
#define BOSS_AWAKENING_DURATION 4
#define BOSS_AWAKENING_SPRITE_COUNT 40
#define BOSS_AWAKEN_DISTANCE 350

#define BOSS_CUTSCENE_SPRITE "img/cutscene_intro.png"
#define BOSS_IDLE_SPRITE "img/boss_idle.png"
#define BOSS_SLAP_RIGHT_SPRITE "img/boss_slap_right.png"
#define BOSS_SLAP_LEFT_SPRITE "img/boss_slap_left.png"
#define BOSS_SLAM_SPRITE "img/boss_slam.png"
#define BOSS_CLAP_SPRITE "img/boss_clap.png"

#define BOSS_CLAP_SOUND "audio/boss/boss_clap.wav"


using namespace std;

class Boss : public Component {
public:
    enum BossState {
        STARTING,
        ATTACKING,
        AWAKENING,
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
    void CreateBars();
    void HideBars();
    void ShowBars();

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
    Timer cutsceneTimer;
    bool awoken;
    
    void SetSprite(string file, bool flip = false);
    void RollingStoneAttack();
    
    double timeToLoadCollider = -1;
    GameObject* colliderToLoad = nullptr;
    Timer colliderTimer;
    weak_ptr<GameObject> healthBar;
    weak_ptr<GameObject> barDecoration;

    void PrintBossState();
    void PrintBossAttack();

    void PlaySound(string file);

};


#endif //NEPHESH_BOSS_H
