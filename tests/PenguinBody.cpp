//
// Created by ftfnunes on 16/04/18.
//

#include <Sprite.h>
#include <PenguinCannon.h>
#include <Game.h>
#include <InputManager.h>
#include <Collider.h>
#include <Bullet.h>
#include <Camera.h>
#include <Sound.h>
#include "PenguinBody.h"

PenguinBody *PenguinBody::player = nullptr;

PenguinBody::PenguinBody(GameObject &associated) : Component(associated), speed(Vec2()), linearSpeed(0), angle(0), hp(50)  {
    associated.AddComponent(new Sprite(associated, "img/penguin.png"));
    associated.AddComponent(new Collider(associated));

    player = this;
}

PenguinBody::~PenguinBody() {
    player = nullptr;
}

void PenguinBody::Update(float dt) {
    if (hp <= 0) {
        associated.RequestDelete();
        if (!pCannon.expired()) {
            (*pCannon.lock()).RequestDelete();
        }

        auto explosionObj = new GameObject(associated.GetLayer());
        auto explosionSprite = new Sprite(*explosionObj, "img/penguindeath.png", 5, 0.1, 0.5);
        auto explosionSound = new Sound(*explosionObj, "audio/boom.wav");
        explosionObj->AddComponent(explosionSprite);
        explosionObj->AddComponent(explosionSound);
        explosionSound->Play();
        explosionObj->SetCenter(associated.box.Center());
        Game::GetInstance().GetCurrentState().AddObject(explosionObj);
    } else {
        auto inputManager = InputManager::GetInstance();

        if (inputManager.KeyPress(SDLK_l)) {
            if (Camera::IsFollowing()) {
                Camera::Unfollow();
            } else {
                Camera::Follow(&associated);
            }
        }

        if (inputManager.IsKeyDown(A_KEY)) {
            angle -= BODY_ANGULAR_SPEED*dt;
        } else if (inputManager.IsKeyDown(D_KEY)) {
            angle += BODY_ANGULAR_SPEED*dt;
        }

        associated.angleDeg = angle;

        double deltaPos;
        auto acceleration = 0.0;
        if (inputManager.IsKeyDown(W_KEY)) {
            if (linearSpeed >= MAX_SPEED) {
                linearSpeed = MAX_SPEED;
            } else {
                acceleration = ACCELERATION;
            }
        } else if (inputManager.IsKeyDown(S_KEY)) {
            if (linearSpeed <= -MAX_SPEED) {
                linearSpeed = -MAX_SPEED;
            } else {
                acceleration = -ACCELERATION;
            }
        }

        auto deltaSpeed = acceleration*dt;
        deltaPos = linearSpeed*dt + deltaSpeed*(dt/2);

        linearSpeed += deltaSpeed;
        
        speed = Vec2(linearSpeed, 0).RotateDeg(angle);
        associated.box += Vec2(deltaPos, 0).RotateDeg(angle);
        associated.box.x = associated.box.x < 0 ? 0 : associated.box.x;
        associated.box.x = associated.box.x > MAP_WIDTH ? MAP_WIDTH : associated.box.x;
        associated.box.y = associated.box.y < 0 ? 0 : associated.box.y;
        associated.box.y = associated.box.y > MAP_HEIGHT ? MAP_HEIGHT : associated.box.y;
    }
}

void PenguinBody::Render() {}

bool PenguinBody::Is(string type) {
    return type == BODY_TYPE;
}

void PenguinBody::Start() {
    auto cannonObject = new GameObject(associated.GetLayer()+1);
    auto associatedPtr = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);
    auto cannon = new PenguinCannon(*cannonObject, associatedPtr);
    cannonObject->AddComponent(cannon);

    pCannon = Game::GetInstance().GetCurrentState().AddObject(cannonObject);
}

void PenguinBody::NotifyCollision(GameObject &other) {
    auto bullet = (Bullet *) other.GetComponent(BULLET_TYPE);
    
    if (bullet != nullptr && bullet->TargetsPlayer()) {
        Damage(bullet->GetDamage());
        cout << "hp" << hp << endl;
    }
}

void PenguinBody::Damage(int damage) {
    hp -= damage;

    if (hp <= 0) {
        Camera::Unfollow();
    }
}

Vec2 PenguinBody::GetPosition() {
    return associated.box.Center();
}


