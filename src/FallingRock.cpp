//
// Created by bruno on 08/07/18.
//

#include <Game.h>
#include <cmath>
#include <Player.h>
#include <Sprite.h>
#include <Collider.h>
#include <Camera.h>
#include <Sound.h>
#include "FallingRock.h"

FallingRock::FallingRock(GameObject &associated) : Component(associated), linearSpeed(0) {
    vector<string> rockPossibleSprites = {FALLING_ROCK_SPRITE_1, FALLING_ROCK_SPRITE_2, FALLING_ROCK_SPRITE_3};

    Vec2 targetReference = (Player::player) ? Player::player->GetCenter() : Camera::Camera::GetCameraCenter();

    targetPos = Vec2(rand() % (int)FALLING_ROCK_MAX_RADIUS, 0).Rotate(M_PI * ((rand() % 1001)/500.0)) + targetReference;

    associated.AddComponent(new Sprite(associated, rockPossibleSprites[rand() % rockPossibleSprites.size()]));
    associated.SetCenter(targetPos - Vec2(0, 1.5*GAME_HEIGHT));

    camShaker = new CameraShaker(associated);
    associated.AddComponent(camShaker);

    rockShadow = new GameObject(associated.GetLayer());
    Game::GetInstance().GetCurrentState().AddObject(rockShadow);
}

void FallingRock::Start() {
    rockShadow->AddComponent(new Sprite(*rockShadow, FALLING_ROCK_SHADOW_SPRITE));
    rockShadow->SetCenter(targetPos);
}

void FallingRock::Update(float dt) {
    if(associated.box.Center() == targetPos){ // Rock is on the ground!
        string fallingRocksSounds[2] = {FALLING_ROCK_BREAKING_SOUND_1, FALLING_ROCK_BREAKING_SOUND_2};

        auto boomGO = new GameObject(associated.GetLayer());
        auto boomSound = new Sound(*boomGO, fallingRocksSounds[rand() % 2]);
        boomGO->AddComponent(boomSound);
        boomSound->Play();
        boomGO->AddComponent(new Sprite(*boomGO, FALLING_ROCK_DUST_SPRITE, 6, 0.1, 0.6));
        boomGO->box.PlaceCenterAt(associated.box.Center());
        Game::GetInstance().GetCurrentState().AddObject(boomGO);

        auto rockBreakingGO = new GameObject(associated.GetLayer());
        rockBreakingGO->AddComponent(new Sprite(*rockBreakingGO, FALLING_ROCK_BREAKING_SPRITE, 7, 0.1, 0.7));
        rockBreakingGO->box.PlaceCenterAt(associated.box.Center());
        Game::GetInstance().GetCurrentState().AddObject(rockBreakingGO);

        camShaker->SingleShake();

        rockShadow->RequestDelete();
        associated.RequestDelete();
    }

    auto deltaSpeed = FALLING_ROCK_ACCELERATION*dt;
    auto deltaPos = linearSpeed*dt + deltaSpeed*(dt/2);
    linearSpeed += deltaSpeed;

    if(associated.box.Center().Distance(targetPos) < deltaPos){
        associated.SetCenter(targetPos);
        associated.AddComponent(new Collider(associated));
    }
    else{
        associated.box += Vec2(0, deltaPos);
    }

}

void FallingRock::Render() {

}

bool FallingRock::Is(string type) {
    return type == FALLING_ROCK_TYPE;
}

void FallingRock::NotifyCollision(GameObject &other) {
//    other.RequestDelete();
}
