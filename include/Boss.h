//
// Created by bruno on 13/06/18.
//

#ifndef NEPHESH_BOSS_H
#define NEPHESH_BOSS_H

#include <algorithm>
#include "GameObject.h"
#include "Vec2.h"
#include "Timer.h"
#include <Sound.h>
#include <Sprite.h>
#include <Game.h>
#include <CameraShaker.h>

#define BOSS_TYPE "Boss"
#define BOSS_INITIAL_HP 100
#define BOSS_IDLE_TIME 5 // in seconds
#define BOSS_ATTACK_TIME 1.0 // in seconds
#define BOSS_MIN_NUM_OF_ATTACKS 1
#define BOSS_MAX_NUM_OF_ATTACKS 1
#define BOSS_SLAP_DISTANCE 400
#define BOSS_IDLE_DISTANCE 1000
#define BOSS_MIN_SLIDING_ROCK_DIST 250
#define BOSS_AWAKENING_DURATION 4
#define BOSS_AWAKENING_SPRITE_COUNT 40
#define BOSS_AWAKEN_DISTANCE 350
#define BOSS_DEATH_DURATION 2.5
#define BOSS_DEATH_SPRITE_COUNT 25
#define BOSS_PROTECT_DURATION 0.2
#define BOSS_PROTECT_SPRITE_COUNT 10

#define BOSS_CUTSCENE_SPRITE "img/cutscene_intro.png"
#define BOSS_IDLE_SPRITE "img/boss_idle.png"
#define BOSS_SLAP_RIGHT_SPRITE "img/boss_slap_right.png"
#define BOSS_SLAP_LEFT_SPRITE "img/boss_slap_left.png"
#define BOSS_SLAM_SPRITE "img/boss_slam.png"
#define BOSS_CLAP_SPRITE "img/boss_clap.png"
#define BOSS_DEATH_SPRITE "img/death.png"
#define BOSS_PROTECT_SPRITE "img/protect.png"
#define BOSS_VULNERABLE_SPRITE "img/vulnerable.png"
#define BOSS_VULNERABLE_SPRITE_COUNT 16
#define BOSS_VULNERABLE_DURATION 2

#define BOSS_CLAP_SOUND "audio/boss/boss_clap.wav"
#define BOSS_SLAM_SOUND "audio/boss/boss_slam.wav"
#define BOSS_SLAM_EARTHQUAKE_SOUND_1 "audio/boss/earthquake_1.wav"
#define BOSS_SLAM_EARTHQUAKE_SOUND_2 "audio/boss/earthquake_2.wav"
#define BOSS_SLAM_EARTHQUAKE_SOUND_3 "audio/boss/earthquake_3.wav"

using namespace std;

class Boss : public Component {
public:
    enum BossState {
        STARTING,
        ATTACKING,
        AWAKENING,
        IDLE,
        DYING,
        VULNERABLE,
        DEFENDING
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

    void DecreaseHp(int damage);
    void TryHitLaser();

    void Start() override;

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
    
    bool work = true;

};


#endif //NEPHESH_BOSS_H
