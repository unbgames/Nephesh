//
// Created by bruno on 13/06/18.
//

#include <Player.h>
#include <MeleeAttack.h>
#include <Collider.h>
#include <RollingStones.h>
#include <Cast.h>
#include <FallingRock.h>
#include <BossMeleeAttack.h>
#include <Debug.h>
#include <Collidable.h>
#include <Camera.h>
#include <WorldState.h>
#include <Bar.h>
#include <CameraFollower.h>
#include <DecentTimer.h>
#include "Boss.h"
#include "Utils.h"

Boss::Boss(GameObject &associated) :
        Component(associated),
        hp(BOSS_INITIAL_HP),
        speed(0, 0),
        currentState(STARTING),
        previousState(STARTING),
        timer(),
        attacksPerformed(0),
        awoken(false) {
    auto sprite = new Sprite(associated, BOSS_CUTSCENE_SPRITE, BOSS_AWAKENING_SPRITE_COUNT,
                             ((float) BOSS_AWAKENING_DURATION) / BOSS_AWAKENING_SPRITE_COUNT);
    sprite->SetScale(2, 2, false);
    associated.AddComponent(sprite);
    sprite->LockFrame();
    associated.AddComponent(new Sound(associated));
    camShaker = new CameraShaker(associated);
    associated.AddComponent(camShaker);
    //Sprite *spr = new Sprite(associated, "img/boss_clap.png", 10, 0.2, 0, true);

//    associated.SetCenter({associated.box.x, associated.box.y});


}

void Boss::Update(float dt) {
    auto center = associated.box.Center();

    if (hp <= 0) {
        if (previousState != DYING) {
            auto oldCenter = associated.box.Center();
            SetSprite(BOSS_DEATH_SPRITE);
            auto sprite = (Sprite *) associated.GetComponent(SPRITE_TYPE);
            sprite->SetScale(2, 2);
            associated.box.PlaceCenterAt(oldCenter);
            cutsceneTimer.Restart();
            PlaySound("audio/boss/boss_morrendo.wav");
            UpdateState(DYING);
        } else {
            cutsceneTimer.Update(dt);
            if (cutsceneTimer.Get() > BOSS_DEATH_DURATION) {
                auto sprite = (Sprite *) associated.GetComponent(SPRITE_TYPE);
                barDecoration.lock()->RequestDelete();
                healthBar.lock()->RequestDelete();
                sprite->LockFrame();
                associated.RequestDelete();
            }
        }

        return;
    }
    //cout << "State: " << currentState << " ||| Center: (" << center.x << ", " << center.y << ")" << endl;

    if (colliderToLoad != nullptr && timeToLoadCollider >= 0) {
        colliderTimer.Update(dt);
    }

    if (Player::player && Player::player->GetHp() > 0 && center.Distance(Player::player->GetCenter()) <=
                                                         BOSS_IDLE_DISTANCE) {
        auto newState = currentState;
        if (newState != STARTING && newState != AWAKENING) {
            Camera::offset = Vec2(0, -100);
        } else if (awoken) {
            Camera::offset = Vec2(0, -200);
        }
        Sprite *sprite;

        switch (currentState) {
            case STARTING:
                if (Player::player && center.Distance(Player::player->GetCenter()) <= BOSS_AWAKEN_DISTANCE) {
                    sprite = (Sprite *) associated.GetComponent(SPRITE_TYPE);
                    sprite->UnlockFrame();
//                    Camera::Follow(&associated);
                    newState = AWAKENING;
                    Player::player->Freeze();
                    PlaySound("audio/boss/boss_acordando.wav");
                    awoken = true;
                }
                break;
            case AWAKENING:
                cutsceneTimer.Update(dt);
                if (cutsceneTimer.Get() > BOSS_AWAKENING_DURATION) {
                    SetSprite(BOSS_IDLE_SPRITE);
//                    Camera::Follow(&Player::player->GetGameObject());
                    Player::player->Unfreeze();
                    newState = IDLE;
                }
                break;
            case DEFENDING:
                if (previousState != DEFENDING) {
                    SetSprite(BOSS_PROTECT_SPRITE);
                    timer.Restart();
                } else {
                    timer.Update(dt);
                    if (timer.Get() > BOSS_PROTECT_DURATION) {
                        newState = IDLE;
                    }
                }
                break;
            case VULNERABLE:
                if (previousState != VULNERABLE) {
                    SetSprite(BOSS_VULNERABLE_SPRITE);
                    timer.Restart();
                } else {
                    timer.Update(dt);
                    if (timer.Get() > BOSS_VULNERABLE_DURATION) {
                        newState = IDLE;
                    }
                }
                break;
            case IDLE:
                ShowBars();
                if (previousState != IDLE) {
                    SetSprite(BOSS_IDLE_SPRITE);
                    timer.Restart();
                } else if (timer.Get() < BOSS_IDLE_TIME) {
                    timer.Update(dt);
                } else {
                    newState = ATTACKING;
                }
                break;
            case ATTACKING:
                if (previousState != ATTACKING) {
                    numOfAttacks = (rand() % (BOSS_MAX_NUM_OF_ATTACKS - BOSS_MIN_NUM_OF_ATTACKS + 1)) +
                                   BOSS_MIN_NUM_OF_ATTACKS;
                    Attack();
                    timer.Restart();
                } else if (timer.Get() < BOSS_ATTACK_TIME) {
                    timer.Update(dt);
                    if (colliderToLoad != nullptr && timeToLoadCollider >= 0 && colliderTimer.Get() >=
                                                                                timeToLoadCollider) {
                        Game::GetInstance().GetCurrentState().AddObject(colliderToLoad);
                        timeToLoadCollider = -1;
                        colliderToLoad = nullptr;
                        colliderTimer.Restart();
                    }

                } else {
                    timer.Restart(0);
                    attacksPerformed++;

                    if (attacksPerformed > 0 || attacksPerformed == numOfAttacks) {
                        attacksPerformed = 0;
                        newState = IDLE;
                        work = false;
                    } else {
                        Attack();
                    }
                }
                break;
        }

        UpdateState(newState);


    } else {
        UpdateState(currentState);
        Camera::offset = Vec2();
        HideBars();
        if (currentState != IDLE && currentState != STARTING && currentState != AWAKENING) {
            SetSprite(BOSS_IDLE_SPRITE);
        }
    }
}

void Boss::Render() {

}

bool Boss::Is(string type) {
    return type == BOSS_TYPE;
}

void Boss::SetSprite(string file, bool flip) {
    auto sprite = (Sprite *) associated.GetComponent("Sprite");
    float wScale = 1, hScale = 1;

    auto lastCenter = associated.box.Center();
    if (!sprite) {
        throw ("Sprite component not found on Boss's GameObject.");
    }

    if (file == BOSS_IDLE_SPRITE) {
        sprite->SetFrameCount(10);
        sprite->SetFrameTime(0.1);
    } else if (file == BOSS_CUTSCENE_SPRITE) {
        sprite->SetFrameCount(BOSS_AWAKENING_SPRITE_COUNT);
        sprite->SetFrameTime(((float) BOSS_AWAKENING_DURATION) / BOSS_AWAKENING_SPRITE_COUNT);
        wScale = 2;
        hScale = 2;
        sprite->LockFrame();
    } else if (file == BOSS_DEATH_SPRITE) {
        sprite->SetFrameCount(BOSS_DEATH_SPRITE_COUNT);
        sprite->SetFrameTime(((float) BOSS_DEATH_DURATION) / BOSS_DEATH_SPRITE_COUNT);
    } else if (file == BOSS_PROTECT_SPRITE) {
        sprite->SetFrameCount(BOSS_PROTECT_SPRITE_COUNT);
        sprite->SetFrameTime(((float) BOSS_PROTECT_DURATION) / BOSS_PROTECT_SPRITE_COUNT);
    } else if (file == BOSS_VULNERABLE_SPRITE) {
        sprite->SetFrameCount(BOSS_VULNERABLE_SPRITE_COUNT);
        sprite->SetFrameTime(((float) BOSS_VULNERABLE_DURATION) / BOSS_VULNERABLE_SPRITE_COUNT);
    } else { // ATTACKS
        sprite->SetFrameCount(10);
        sprite->SetFrameTime(BOSS_ATTACK_TIME / 10);
    }

    sprite->SetFlip(flip);
    sprite->SetScale(wScale, hScale, false);
    sprite->Open(file, false);
    sprite->SetFrame(0);

    associated.box.PlaceCenterAt(lastCenter);
}


void Boss::Attack() {
    auto playerCenter = Player::player->GetCenter();
    auto bossCenter = associated.box.Center();
    auto dist = bossCenter.Distance(playerCenter);
    int numOfAttacks = 3;

    vector<int> attackProbabilityWeights(numOfAttacks);

    if (dist <= BOSS_SLAP_DISTANCE) {
        attackProbabilityWeights[SLAP] = 80;
        attackProbabilityWeights[SLAM] = 10;
        attackProbabilityWeights[CLAP] = 10;
    } else {
        attackProbabilityWeights[SLAP] = 0;
        attackProbabilityWeights[SLAM] = 40;
        attackProbabilityWeights[CLAP] = 60;
    }

    attackState = (BossAttack) WeightedDraft(attackProbabilityWeights);

    switch (attackState) {
        case SLAP:
            SlapAttack();
            break;
        case SLAM:
            SlamAttack();
            break;
        case CLAP:
            ClapAttack();
            break;
    }
}

void Boss::UpdateState(Boss::BossState newState) {
    previousState = currentState;
    currentState = newState;
}

void Boss::SlapAttack() {
    auto playerCenter = Player::player->GetCenter();
    auto bossCenter = associated.box.Center();
    Vec2 bossBoxPos;
    int numOfSounds = 2;
    
    vector<string> soundsVector = {"audio/boss/boss_slap_1.wav", "audio/boss/boss_slap_2.wav" };
    vector<int> soundProbabilityWeights(numOfSounds);

    soundProbabilityWeights[0] = 50;
    soundProbabilityWeights[1] = 50;

    int soundIndex = WeightedDraft(soundProbabilityWeights);

    auto attackObject = new GameObject(0);
    attackObject->AddComponent(new BossMeleeAttack(*attackObject, 15, "", 0, BOSS_ATTACK_TIME));
    auto airWaveMeleeObject = new GameObject(0);

    if (playerCenter.x <= bossCenter.x) { //LEFT
        SetSprite(BOSS_SLAP_LEFT_SPRITE);
        bossBoxPos = Vec2(associated.box.x, associated.box.y);
        attackObject->box = Rect(associated.box.h / 2, associated.box.w);
        attackObject->box = bossBoxPos + Vec2(associated.box.w / 12, associated.box.h / 5);
        attackObject->angleDeg = 45;

        colliderTimer.Restart();

        auto secondsToEndAttack = 0.5;
        timeToLoadCollider = BOSS_ATTACK_TIME - secondsToEndAttack;
        auto attackCenter = attackObject->box.Center();
        airWaveMeleeObject->box.PlaceCenterAt({attackCenter.x - 270, attackCenter.y + 270});
        airWaveMeleeObject->angleDeg = -50;
        airWaveMeleeObject->AddComponent(new BossMeleeAttack(*airWaveMeleeObject, 10, "img/slash_boss.png", 4,
                                                             secondsToEndAttack));
        colliderToLoad = airWaveMeleeObject;
    } else { //RIGHT
        SetSprite(BOSS_SLAP_RIGHT_SPRITE);
        bossBoxPos = Vec2(associated.box.x, associated.box.y);
        attackObject->box = Rect(associated.box.h / 2, associated.box.w);
        attackObject->box = bossBoxPos + Vec2(-associated.box.w / 12, associated.box.h / 5);
        attackObject->rotationCenter = Vec2(associated.box.w, 0);
        attackObject->angleDeg = -45;

        colliderTimer.Restart();

        auto secondsToEndAttack = 0.5;
        timeToLoadCollider = BOSS_ATTACK_TIME - secondsToEndAttack;
        auto attackCenter = attackObject->box.Center();
        airWaveMeleeObject->box.PlaceCenterAt({attackCenter.x + 270, attackCenter.y + 270});
        airWaveMeleeObject->angleDeg = 50;
        airWaveMeleeObject->AddComponent(
                new BossMeleeAttack(*airWaveMeleeObject, 10, "img/slash_boss.png", 4, secondsToEndAttack,
                                    true));
        colliderToLoad = airWaveMeleeObject;
        //airWaveMeleeObject->AddComponent(new Debug(*airWaveMeleeObject));
        //Game::GetInstance().GetCurrentState().AddObject(airWaveMeleeObject);
    }

    auto sound = new Sound(*attackObject, soundsVector[soundIndex]);
    attackObject->AddComponent(sound);
    sound->Play();
    Game::GetInstance().GetCurrentState().AddObject(attackObject);
}

void Boss::SlamAttack() {
    SetSprite(BOSS_SLAM_SPRITE);

    const int ATTACK_RANGE = 0.6 * associated.box.h;
    const int ATTACK_WIDTH = 0.7 * associated.box.w;

    auto bossBoxPos = Vec2(associated.box.x, associated.box.y);

    colliderTimer.Restart();

    auto secondsToEndAttack = 0.5;
    timeToLoadCollider = BOSS_ATTACK_TIME - secondsToEndAttack;

    colliderToLoad = new GameObject(0);
    colliderToLoad->AddComponent(new BossMeleeAttack(*colliderToLoad, 20, "", 0, secondsToEndAttack));
    colliderToLoad->box = Rect(ATTACK_RANGE, ATTACK_WIDTH);
    colliderToLoad->box = bossBoxPos + Vec2(0.15 * associated.box.w, 0.55 * associated.box.h);

    auto thisGO = &associated;
    DecentTimer::ProgramTimer(associated, 0.2, [thisGO]() -> void {
        auto boss = (Boss *) thisGO->GetComponent(BOSS_TYPE);
        boss->PlaySound(BOSS_SLAM_SOUND);
    });

    DecentTimer::ProgramTimer(associated, BOSS_ATTACK_TIME - 0.3, [thisGO]() -> void {
        string earthquakeSounds[3] = {BOSS_SLAM_EARTHQUAKE_SOUND_1, BOSS_SLAM_EARTHQUAKE_SOUND_2,
                                      BOSS_SLAM_EARTHQUAKE_SOUND_3};

        auto boss = (Boss *) thisGO->GetComponent(BOSS_TYPE);
        boss->PlaySound(earthquakeSounds[rand() % 3]);
        boss->camShaker->KeepShaking(BOSS_ATTACK_TIME, true);
    });

    RockSlide();
}

void Boss::PrintBossState() {
    cout << "Current Boss State: ";
    switch (currentState) {
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
    switch (attackState) {
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

    for (int i = 0; i < numOfRocks; i++) {
        auto rockGO = new GameObject(0);
        rockGO->AddComponent(new FallingRock(*rockGO, 20));

        auto rockCastGO = new GameObject(0);
        rockCastGO->AddComponent(new Cast(*rockCastGO, rockGO, BOSS_ATTACK_TIME + i * TIME_BETWEEN_ROCKS));

        Game::GetInstance().GetCurrentState().AddObject(rockCastGO);
    }
}

void Boss::PlaySound(string file) {
    auto sound = (Sound *) associated.GetComponent(SOUND_TYPE);
    sound->Open(file);
    sound->Play();
}


void Boss::RollingStoneAttack() {
    auto offWid = 800;
    auto offHeig = 1000;
    auto bossCenter = associated.box.Center();
    auto playerCenter = Player::player->GetCenter();

    auto rs = new GameObject(0);

    auto heightOffset = rand() % offHeig;
    auto widthOffset = (rand() % offWid) - offWid / 2;

    Vec2 blockPos = {bossCenter.x + widthOffset, bossCenter.y + (associated.box.h / 2) + 10 + heightOffset};

    if (blockPos.Distance(playerCenter) < BOSS_MIN_SLIDING_ROCK_DIST) {
        Vec2 topRight = {bossCenter.x + offWid / 2, bossCenter.y + (associated.box.h / 2) + 10};
        Vec2 topLeft = {bossCenter.x - offWid / 2, bossCenter.y + (associated.box.h / 2) + 10};
        Vec2 botRight = {bossCenter.x + offWid / 2, bossCenter.y + (associated.box.h / 2) + 10 + offHeig};
        Vec2 botLeft = {bossCenter.x - offWid / 2, bossCenter.y + (associated.box.h / 2) + 10 + offHeig};

        auto topRightDist = playerCenter.Distance(topRight);
        auto topLeftDist = playerCenter.Distance(topLeft);
        auto botRightDist = playerCenter.Distance(botRight);
        auto botLeftDist = playerCenter.Distance(botLeft);

        float dist = std::max({topRightDist, topLeftDist, botRightDist, botLeftDist}, cmp);

        if (dist == topRightDist) {
            blockPos = topRight;
        } else if (dist == topLeftDist) {
            blockPos = topLeft;
        } else if (dist == botRightDist) {
            blockPos = botRight;
        } else if (dist == botLeftDist) {
            blockPos = botLeft;
        }
    }

    rs->box = blockPos;
    rs->AddComponent(new RollingStones(*rs, 10, 300, 5200));

    auto chargeTime = 0.7;
    auto chargeObj = new GameObject(0);
    chargeObj->AddComponent(new Cast(*chargeObj, rs, chargeTime));
    auto rsSprite = new Sprite(*chargeObj, RSTONE_SLIDING_UP_STONE_SPRITE, 6, chargeTime / 6);
    chargeObj->AddComponent(rsSprite);
    chargeObj->box = rs->box;
    auto *sound = new Sound(*chargeObj, "audio/boss/boss_earthbend.wav");
    chargeObj->AddComponent(sound);
    sound->Play();
    Game::GetInstance().GetCurrentState().AddObject(chargeObj);

    auto chargeObj2 = new GameObject(0);
    chargeObj2->AddComponent(new Cast(*chargeObj2, nullptr, chargeTime));
    auto dustSprite = new Sprite(*chargeObj2, "img/poeira.png", 6, chargeTime / 6);
    chargeObj2->AddComponent(dustSprite);
    auto dustPos = chargeObj->box.Center();
    dustPos.y += 60;
    chargeObj2->SetCenter(dustPos);
    Game::GetInstance().GetCurrentState().AddObject(chargeObj2);
}

void Boss::ClapAttack() {

    colliderTimer.Restart();

    auto secondsToEndAttack = 0.4;
    timeToLoadCollider = BOSS_ATTACK_TIME - secondsToEndAttack;
    colliderToLoad = new GameObject(0);

    auto playerBoxCenter = associated.box.Center();

    colliderToLoad->SetCenter(playerBoxCenter);
    colliderToLoad->AddComponent(new BossMeleeAttack(*colliderToLoad, 15, "img/slash_boss_down.png", 4,
                                                     secondsToEndAttack, true, {0, 0}, {1.2, 0.7}));

    colliderToLoad->box.y += associated.box.h / 2 + 40;

    SetSprite(BOSS_CLAP_SPRITE);
    int numOfRollingStones = (rand() % 4) + 1;
    while (numOfRollingStones > 0) {
        RollingStoneAttack();
        --numOfRollingStones;
    }

    PlaySound(BOSS_CLAP_SOUND);
}

void Boss::CreateBars() {
    auto &state = (WorldState &) Game::GetInstance().GetCurrentState();

    auto healthBarObject = new GameObject(HUD_LAYER);
    healthBarObject->AddComponent(new Bar(*healthBarObject, "img/hp_boss.png", hp, BOSS_INITIAL_HP));
    auto healthBarPosition = Vec2(GAME_WIDTH / 2 - healthBarObject->box.w / 2, GAME_HEIGHT - 60);
    healthBarObject->AddComponent(new CameraFollower(*healthBarObject, healthBarPosition));
    healthBar = state.AddObject(healthBarObject);

    auto decoration = new GameObject(HUD_LAYER);
    decoration->AddComponent(new Sprite(*decoration, "img/deco_boss_escuro.png"));
    auto rPosition = Vec2(GAME_WIDTH / 2 - decoration->box.w / 2,
                          healthBarPosition.y - healthBarObject->box.h / 2 - 20);
    healthBarObject->AddComponent(new CameraFollower(*decoration, rPosition));
    barDecoration = state.AddObject(decoration);
}

void Boss::HideBars() {
    auto decorationSprite = (Sprite *) barDecoration.lock()->GetComponent(SPRITE_TYPE);
    decorationSprite->SetAlpha(0);
    auto barSprite = (Sprite *) healthBar.lock()->GetComponent(SPRITE_TYPE);
    barSprite->SetAlpha(0);
}

void Boss::ShowBars() {
    auto decorationSprite = (Sprite *) barDecoration.lock()->GetComponent(SPRITE_TYPE);
    decorationSprite->SetAlpha(255);
    auto barSprite = (Sprite *) healthBar.lock()->GetComponent(SPRITE_TYPE);
    barSprite->SetAlpha(255);
}

void Boss::Start() {
    CreateBars();
    HideBars();
}

void Boss::DecreaseHp(int damage) {
    if (currentState == VULNERABLE) {
        hp -= damage;
        if (hp < 0) {
            hp = 0;
        }
        auto bar = (Bar *) healthBar.lock()->GetComponent(BAR_TYPE);
        bar->SetValue(hp);
    }
}

void Boss::TryHitLaser() {
    if (awoken) {
        if (/*currentState == IDLE*/currentState == ATTACKING && attackState == CLAP) {
            PlaySound("audio/hitbox_magica.wav");
            UpdateState(VULNERABLE);
        } else {
            PlaySound("audio/hitbox_magica_fail.wav");
            UpdateState(DEFENDING);
        }
    }

//    if (awoken) {
//        if (currentState == IDLE) {
//            PlaySound("audio/hitbox_magica.wav");
//            UpdateState(VULNERABLE);
//        } else {
//            PlaySound("audio/hitbox_magica_fail.wav");
//            UpdateState(DEFENDING);
//        }
//    }
    
    
}

int Boss::GetHp() {
    return hp;
}

void Boss::SetHp(int h) {
    hp = h;
}

void Boss::Restart() {
    hp = 100;
    SetSprite(BOSS_CUTSCENE_SPRITE);
    currentState = STARTING;
    awoken = false;
    Start();
}

