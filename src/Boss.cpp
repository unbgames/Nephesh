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
        bossState(IDLE),
        timer(),
        attacksPerformed(0) {

    Sprite *spr = new Sprite(associated, "img/boss_walk_down_sheet.png", 4, 100000000, 0, true);

    associated.AddComponent(spr);
    associated.box.h = spr->GetHeight();
    associated.box.w = spr->GetWidth();

    associated.SetCenter({associated.box.x, associated.box.y});
    
    movementAnimations.emplace_back(make_pair(RIGHT, "img/boss_walk_side_sheet.png"));
    movementAnimations.emplace_back(make_pair(LEFT, "img/boss_walk_side_sheet.png"));
    movementAnimations.emplace_back(make_pair(UP, "img/boss_walk_up_sheet.png"));
    movementAnimations.emplace_back(make_pair(DOWN, "img/boss_walk_down_sheet.png"));

    attackAnimations.emplace_back(make_pair(RIGHT, "img/boss_attack_side_sheet.png"));
    attackAnimations.emplace_back(make_pair(LEFT, "img/boss_attack_side_sheet.png"));
    attackAnimations.emplace_back(make_pair(UP, "img/boss_attack_up_sheet.png"));
    attackAnimations.emplace_back(make_pair(DOWN, "img/boss_attack_down_sheet.png"));
    
    currentDirection = DOWN;
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

    auto oldDirection = currentDirection;
    currentDirection = GetNewDirection();

    auto center = associated.box.Center();
    cout << "State: " << bossState << " ||| Center: (" << center.x << ", " << center.y << ")" << endl;

    if(Player::player){
        if(bossState == IDLE && timer.Get() < BOSS_IDLE_TIME){
            timer.Update(dt);

            if(oldDirection != currentDirection){
                SetSprite(GetMovementAnimation(), 4, 100000000, currentDirection == LEFT);
            }
        }
        else if(bossState == IDLE){
            speed = {BOSS_SPEED, 0};
            speed = speed.Rotate(associated.box.Center().DiferenceAngle(Player::player->GetCenter()));

            bossState = MOVING;
            SetSprite(GetMovementAnimation(), 4, BOSS_SPR_MOV_TIME, currentDirection == LEFT);
        }
        else if(bossState == MOVING && Player::player->GetCenter().Distance(associated.box.Center()) <= BOSS_MIN_DIST_TO_PLAYER){
            bossState = ATTACKING;

            Attack();

            timer.Restart(0);
        }
        else if(bossState == MOVING){
            speed = {BOSS_SPEED, 0};
            speed = speed.Rotate(associated.box.Center().DiferenceAngle(Player::player->GetCenter()));

            associated.box += speed*dt;
            
            if(oldDirection != currentDirection){
                SetSprite(GetMovementAnimation(), 4, BOSS_SPR_MOV_TIME, currentDirection == LEFT);
            }
        }
        else if(bossState == ATTACKING && timer.Get() < BOSS_ATTACK_TIME){
            timer.Update(dt);
        }
        else if(bossState == ATTACKING){
            timer.Restart(0);
            attacksPerformed++;

            if(attacksPerformed == BOSS_NUM_OF_ATTACKS){
                attacksPerformed = 0;
                SetSprite(GetMovementAnimation(), 4, 100000000, currentDirection == LEFT);
                bossState = IDLE;
            }
            else{
                Attack();
            }
        }
    }

}

void Boss::Render() {

}

bool Boss::Is(string type) {
    return type == BOSS_TYPE;
}

string Boss::GetMovementAnimation() {
    for (auto &animation : movementAnimations) {
        if (animation.first == currentDirection) {
            return animation.second;
        }
    }

    return nullptr; // not supposed to happen
}

string Boss::GetAttackAnimation() {
    for (auto &animation : attackAnimations) {
        if (animation.first == currentDirection) {
            return animation.second;
        }
    }

    return nullptr; // not supposed to happen
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

Boss::BossDirection Boss::GetNewDirection() {
    if(Player::player){
        
        auto target = Player::player->GetCenter();
        auto angle = (target - associated.box.Center()).XAngleDeg();

        if(angle > -60 && angle < 60){
            return RIGHT;
        } else if (angle > 60 && angle < 120) {
            return DOWN;
        } else if (angle > 120 || angle < -120) {
            return LEFT;
        } else {
            return UP;
        }
    
    }
    
    return DOWN;
}

void Boss::Attack() {
    SetSprite(GetAttackAnimation(), BOSS_ATTACK_SPRITE_COUNT, BOSS_ATTACK_TIME/BOSS_ATTACK_SPRITE_COUNT, currentDirection == LEFT);

    auto attackObject = new GameObject(associated.GetLayer());
    attackObject->AddComponent(new MeleeAttack(*attackObject, BOSS_ATTACK_TIME));
    auto bossBoxPosition = Vec2(associated.box.x, associated.box.y);

    if (currentDirection == RIGHT) {
        attackObject->box = Rect(BOSS_ATTACK_WIDTH, BOSS_ATTACK_RANGE);
        auto offset = Vec2(-associated.box.w/5, 0);
        attackObject->box = bossBoxPosition + Vec2(associated.box.w, (associated.box.h - attackObject->box.h)/2) + offset;
    } else if (currentDirection == DOWN) {
        attackObject->box = Rect(BOSS_ATTACK_RANGE, BOSS_ATTACK_WIDTH);
        auto offset = Vec2(0, -associated.box.h/5);
        attackObject->box = bossBoxPosition + Vec2((associated.box.w - attackObject->box.w)/2, associated.box.h) + offset;
    } else if (currentDirection == LEFT) {
        attackObject->box = Rect(BOSS_ATTACK_WIDTH, BOSS_ATTACK_RANGE);
        auto offset = Vec2(associated.box.w/5, 0);
        attackObject->box = bossBoxPosition + Vec2(-attackObject->box.w, (associated.box.h - attackObject->box.h)/2) + offset;
    } else {
        attackObject->box = Rect(BOSS_ATTACK_RANGE, BOSS_ATTACK_WIDTH);
        auto offset = Vec2(0, associated.box.h/5);
        attackObject->box = bossBoxPosition + Vec2((associated.box.w - attackObject->box.w)/2, -attackObject->box.h) + offset;
    }

    Game::GetInstance().GetCurrentState().AddObject(attackObject);
}
