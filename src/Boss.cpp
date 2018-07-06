//
// Created by bruno on 13/06/18.
//

#include <Player.h>
#include <MeleeAttack.h>
#include <Collider.h>
#include "Boss.h"

Boss::Boss(GameObject &associated) :
        Component(associated),
        hp(BOSS_INITIAL_HP),
        speed(0,0),
        currentState(IDLE),
        oldState(IDLE),
        timer(),
        attacksPerformed(0) {

    Sprite *spr = new Sprite(associated, BOSS_SLAP_LEFT_SPRITE, 10, 0.1, 0, true);
    //Sprite *spr = new Sprite(associated, "img/boss_clap.png", 10, 0.2, 0, true);

    associated.AddComponent(spr);
    associated.box.h = spr->GetHeight();
    associated.box.w = spr->GetWidth();

    associated.SetCenter({associated.box.x, associated.box.y});
}

void Boss::Update(float dt) {
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
                if(oldState == ATTACKING){
                    SetSprite(BOSS_IDLE_SPRITE, 10, 0.1);
                    timer.Restart();
                } else if(timer.Get() < BOSS_IDLE_TIME){
                    timer.Update(dt);
                } else {
                    newState = ATTACKING;
                }
                break;
                
            case ATTACKING:
                if(oldState == IDLE){
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
        SetSprite(BOSS_IDLE_SPRITE, 10, 0.1);
    }
    

}

void Boss::Render() {

}

bool Boss::Is(string type) {
    return type == BOSS_TYPE;
}

void Boss::SetSprite(string file, int frameCount, float frameTime, bool flip) {
    auto sprite = (Sprite*)associated.GetComponent("Sprite");

    if(!sprite){
        throw("Sprite component not found on Boss's GameObject.");
    }

    sprite->SetFlip(flip);
    sprite->SetFrameCount(frameCount);
    sprite->SetFrameTime(frameTime);
    sprite->Open(file);
    sprite->SetFrame(0);
}


void Boss::Attack() {
    auto playerBox = Player::player->GetCenter();
    auto bossBox = associated.box.Center();
    auto dist = bossBox.Distance(playerBox);    
    
    
    if(dist <= BOSS_SLAP_DISTANCE){
        auto sprites = vector<string>();
        if(playerBox.x <= bossBox.x){
            sprites.emplace_back(BOSS_SLAP_LEFT_SPRITE);
            sprites.emplace_back(BOSS_SLAP_LEFT_SPRITE);
            sprites.emplace_back(BOSS_SLAP_LEFT_SPRITE);
            sprites.emplace_back(BOSS_SLAM_SPRITE);
            sprites.emplace_back(BOSS_CLAP_SPRITE);
        } else{
            sprites.emplace_back(BOSS_SLAP_RIGHT_SPRITE);
            sprites.emplace_back(BOSS_SLAP_RIGHT_SPRITE);
            sprites.emplace_back(BOSS_SLAP_RIGHT_SPRITE);
            sprites.emplace_back(BOSS_SLAM_SPRITE);
            sprites.emplace_back(BOSS_CLAP_SPRITE);
        }
        
        string attack = sprites[rand()%sprites.size()];
        
        SetSprite(attack, 10, BOSS_ATTACK_TIME/10);
                
    } else {
        if(rand()%100 < 50){
            SetSprite(BOSS_SLAM_SPRITE, 10, BOSS_ATTACK_TIME/10);
//            attackObject->box = Rect(BOSS_ATTACK_RANGE, BOSS_ATTACK_WIDTH);
//            auto offset = Vec2(0, -associated.box.h/5);
//            attackObject->box = bossBoxPosition + Vec2((associated.box.w - attackObject->box.w)/2, associated.box.h) + offset;
        } else{
            SetSprite(BOSS_CLAP_SPRITE, 10, BOSS_ATTACK_TIME/10);
//            attackObject->box = Rect(BOSS_ATTACK_RANGE, BOSS_ATTACK_WIDTH);
//            auto offset = Vec2(0, -associated.box.h/5);
//            attackObject->box = bossBoxPosition + Vec2((associated.box.w - attackObject->box.w)/2, associated.box.h) + offset;
        }
    }
//    SetSprite(GetAttackAnimation(), BOSS_ATTACK_SPRITE_COUNT, BOSS_ATTACK_TIME/BOSS_ATTACK_SPRITE_COUNT, currentDirection == LEFT);
//
//    auto attackObject = new GameObject(associated.GetLayer());
//    attackObject->AddComponent(new MeleeAttack(*attackObject, BOSS_ATTACK_TIME));
//    auto bossBoxPosition = Vec2(associated.box.x, associated.box.y);
//
//    if (currentDirection == RIGHT) {
//        attackObject->box = Rect(BOSS_ATTACK_WIDTH, BOSS_ATTACK_RANGE);
//        auto offset = Vec2(-associated.box.w/5, 0);
//        attackObject->box = bossBoxPosition + Vec2(associated.box.w, (associated.box.h - attackObject->box.h)/2) + offset;
//    } else if (currentDirection == DOWN) {
//        attackObject->box = Rect(BOSS_ATTACK_RANGE, BOSS_ATTACK_WIDTH);
//        auto offset = Vec2(0, -associated.box.h/5);
//        attackObject->box = bossBoxPosition + Vec2((associated.box.w - attackObject->box.w)/2, associated.box.h) + offset;
//    } else if (currentDirection == LEFT) {
//        attackObject->box = Rect(BOSS_ATTACK_WIDTH, BOSS_ATTACK_RANGE);
//        auto offset = Vec2(associated.box.w/5, 0);
//        attackObject->box = bossBoxPosition + Vec2(-attackObject->box.w, (associated.box.h - attackObject->box.h)/2) + offset;
//    } else {
//        attackObject->box = Rect(BOSS_ATTACK_RANGE, BOSS_ATTACK_WIDTH);
//        auto offset = Vec2(0, associated.box.h/5);
//        attackObject->box = bossBoxPosition + Vec2((associated.box.w - attackObject->box.w)/2, -attackObject->box.h) + offset;
//    }
//
//    Game::GetInstance().GetCurrentState().AddObject(attackObject);
}

void Boss::UpdateState(Boss::BossState newState) {
    oldState = currentState;
    currentState = newState;
}
