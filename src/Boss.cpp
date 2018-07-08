//
// Created by bruno on 13/06/18.
//

#include <Player.h>
#include <MeleeAttack.h>
#include <Collider.h>
#include <Charge.h>
#include <FallingRock.h>
#include <InputManager.h>
#include "Boss.h"
#include "Utils.h"

Boss::Boss(GameObject &associated) :
        Component(associated),
        hp(BOSS_INITIAL_HP),
        speed(0,0),
        currentState(IDLE),
        previousState(IDLE),
        timer(),
        attacksPerformed(0) {
    associated.AddComponent(new Sprite(associated, BOSS_IDLE_SPRITE, 10, 0.1));
    associated.AddComponent(new Sound(associated));

    camShaker = new CameraShaker(associated);
    associated.AddComponent(camShaker);

    associated.SetCenter({associated.box.x, associated.box.y});
}

void Boss::Update(float dt) {

    auto& inputManager = InputManager::GetInstance();

    if(inputManager.KeyPress(SDLK_q)){
        camShaker->KeepShaking(3);
    }
    if(inputManager.KeyPress(SDLK_e)){
        camShaker->KeepShaking(3, true);
    }

    if(hp <= 0){
        associated.RequestDelete();

        auto explosionGO = new GameObject();
        auto explosionSound = new Sound(*explosionGO, "BOOM.wav");
        explosionGO->AddComponent(new Sprite(*explosionGO, "EXPLOSION.png", 4, 0.1, 0.4));
        explosionGO->AddComponent(explosionSound);
        explosionSound->Play();
        explosionGO->box.PlaceCenterAt(associated.box.Center());
        Game::GetInstance().GetCurrentState().AddObject(explosionGO);

        return;
    }

    auto center = associated.box.Center();
    //cout << "State: " << currentState << " ||| Center: (" << center.x << ", " << center.y << ")" << endl;
    
    if(Player::player){
        auto newState = currentState;
        
        switch(currentState){
            case IDLE:
                if(previousState == ATTACKING){
                    SetSprite(BOSS_IDLE_SPRITE);
                    timer.Restart();
                } else if(timer.Get() < BOSS_IDLE_TIME){
                    timer.Update(dt);
                } else {
                    newState = ATTACKING;
                }
                break;
                
            case ATTACKING:
                if(previousState == IDLE){
                    numOfAttacks = (rand()%(BOSS_MAX_NUM_OF_ATTACKS - BOSS_MIN_NUM_OF_ATTACKS + 1)) + 
                    BOSS_MIN_NUM_OF_ATTACKS;
                    Attack();
                    timer.Restart();
                } else if( timer.Get() < BOSS_ATTACK_TIME){
                    timer.Update(dt);
                } else {
                    timer.Restart(0);
                    attacksPerformed++;

                    if (attacksPerformed == numOfAttacks) {
                        attacksPerformed = 0;
                        newState = IDLE;
                    } else {
                        Attack();
                    }
                }
                break;
        }
        
        UpdateState(newState);
        

    } else{
        UpdateState(IDLE);
        SetSprite(BOSS_IDLE_SPRITE);
    }
    

}

void Boss::Render() {

}

bool Boss::Is(string type) {
    return type == BOSS_TYPE;
}

void Boss::SetSprite(string file, bool flip) {
    auto sprite = (Sprite*)associated.GetComponent("Sprite");

    if(!sprite){
        throw("Sprite component not found on Boss's GameObject.");
    }

    if(file == BOSS_IDLE_SPRITE){
        sprite->SetFrameCount(10);
        sprite->SetFrameTime(0.1);
    }
    else{ // ATTACKS
        sprite->SetFrameCount(10);
        sprite->SetFrameTime(BOSS_ATTACK_TIME/10);
    }

    sprite->SetFlip(flip);
    sprite->Open(file);
    sprite->SetFrame(0);
}


void Boss::Attack() {
//    auto playerCenter = Player::player->GetCenter();
//    auto bossCenter = associated.box.Center();
//    auto dist = bossCenter.Distance(playerCenter);
//
//    int numOfAttacks = 3;
//
//    vector<int> attackProbabilityWeights(numOfAttacks);
//
//    if(dist <= BOSS_SLAP_DISTANCE){
//        attackProbabilityWeights[SLAP] = 60;
//        attackProbabilityWeights[SLAM] = 20;
//        attackProbabilityWeights[CLAP] = 20;
//    }
//    else{
//        attackProbabilityWeights[SLAP] = 0;
//        attackProbabilityWeights[SLAM] = 50;
//        attackProbabilityWeights[CLAP] = 50;
//    }
//
//    attackState = (BossAttack) WeightedDraft(attackProbabilityWeights);
//
//    switch(attackState){
//        case SLAP:
//            SlapAttack();
//            break;
//        case SLAM:
//            SlamAttack();
//            break;
//        case CLAP:
//            ClapAttack();
//            break;
//    }
}

void Boss::UpdateState(Boss::BossState newState) {
    previousState = currentState;
    currentState = newState;
}

void Boss::SlapAttack() {
    auto playerCenter = Player::player->GetCenter();
    auto bossCenter = associated.box.Center();
    Vec2 bossBoxPos;

    auto attackObject = new GameObject(associated.GetLayer());
    attackObject->AddComponent(new MeleeAttack(*attackObject, BOSS_ATTACK_TIME));

    if(playerCenter.x <= bossCenter.x) { //LEFT
        SetSprite(BOSS_SLAP_LEFT_SPRITE);
        bossBoxPos = Vec2(associated.box.x, associated.box.y);
        attackObject->box = Rect(associated.box.h/2, associated.box.w);
        attackObject->box = bossBoxPos + Vec2(associated.box.w/12, associated.box.h/5);
        attackObject->angleDeg = 45;
    }
    else{ //RIGHT
        SetSprite(BOSS_SLAP_RIGHT_SPRITE);
        bossBoxPos = Vec2(associated.box.x, associated.box.y);
        attackObject->box = Rect(associated.box.h/2, associated.box.w);
        attackObject->box = bossBoxPos + Vec2(-associated.box.w/12, associated.box.h/5);
        attackObject->rotationCenter = Vec2(associated.box.w, 0);
        attackObject->angleDeg = -45;
    }

    Game::GetInstance().GetCurrentState().AddObject(attackObject);
}

void Boss::SlamAttack() {
    SetSprite(BOSS_SLAM_SPRITE);

    const int ATTACK_RANGE = 0.6*associated.box.h;
    const int ATTACK_WIDTH = 0.7*associated.box.w;

//    auto playerCenter = Player::player->GetCenter();
//    auto bossCenter = associated.box.Center();
    auto bossBoxPos = Vec2(associated.box.x, associated.box.y);

    auto attackObject = new GameObject(associated.GetLayer());
    attackObject->AddComponent(new MeleeAttack(*attackObject, BOSS_ATTACK_TIME));
    attackObject->box = Rect(ATTACK_RANGE, ATTACK_WIDTH);
    attackObject->box = bossBoxPos + Vec2(0.15*associated.box.w, 0.55*associated.box.h);

    Game::GetInstance().GetCurrentState().AddObject(attackObject);

    RockSlide();
}

void Boss::ClapAttack() {

}

void Boss::PrintBossState() {
    cout << "Current Boss State: ";
    switch (currentState){
        case ATTACKING:
            cout << "ATTACKING" << endl;
            break;
        case IDLE:
            cout << "IDLE" << endl;
            break;
    }
}

void Boss::PrintBossAttack() {
    cout << "Current Boss Attack State: ";
    switch (attackState){
        case SLAM:
            cout << "SLAM ATTACK" << endl;
            break;
        case SLAP:
            cout << "SLAP ATTACK" << endl;
            break;
        case CLAP:
            cout << "CLAP ATTACK" << endl;
            break;
    }
}

void Boss::RockSlide() {
    const int MIN_NUM_OF_ROCKS = 5;
    const int MAX_NUM_OF_ROCKS = 10;
    const float TIME_BETWEEN_ROCKS = 0.1;

    int numOfRocks = (rand() % (MAX_NUM_OF_ROCKS - MIN_NUM_OF_ROCKS + 1)) + MIN_NUM_OF_ROCKS;

    for(int i = 0; i < numOfRocks; i++){
        auto rockGO = new GameObject(associated.GetLayer());
        rockGO->AddComponent(new FallingRock(*rockGO));

        auto rockCastGO = new GameObject(associated.GetLayer());
        rockCastGO->AddComponent(new Charge(*rockCastGO, rockGO, BOSS_ATTACK_TIME + i*TIME_BETWEEN_ROCKS));

        Game::GetInstance().GetCurrentState().AddObject(rockCastGO);
    }
}

void Boss::PlaySound(string file) {
    auto sound = (Sound *) associated.GetComponent(SOUND_TYPE);
    sound->Open(file);
    sound->Play();
}
