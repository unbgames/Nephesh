//
// Created by ftfnunes on 16/04/18.
//

#include <Sprite.h>
#include <InputManager.h>
#include <Camera.h>
#include <Game.h>
#include <Bullet.h>
#include <Collider.h>
#include <PenguinBody.h>
#include "PenguinCannon.h"


PenguinCannon::PenguinCannon(GameObject &associated, weak_ptr<GameObject> penguinBody) : Component(associated), pBody(penguinBody), angle(0) {
    associated.AddComponent(new Sprite(associated, "img/cubngun.png"));
    associated.AddComponent(new Collider(associated));
}

void PenguinCannon::Update(float dt) {
    timer.Update(dt);
    if (pBody.expired()) {
        associated.RequestDelete();
    } else {
        auto bodyCenter = (*pBody.lock()).box.Center();
        associated.SetCenter(bodyCenter);

        auto inputManager = InputManager::GetInstance();
        auto mousePos = Camera::GetAbsolutePosition(associated.GetLayer(), inputManager.GetMouse());

        angle = associated.angleDeg = (mousePos - bodyCenter).XAngleDeg();

        if (inputManager.IsMouseDown(LEFT_MOUSE_BUTTON) && timer.Get() > SHOOTING_COOLDOWN) {
            Shoot();
            timer.Restart();
        }
    }
}

void PenguinCannon::Render() {}

bool PenguinCannon::Is(string type) {
    return type == CANNON_TYPE;
}

void PenguinCannon::Shoot() {
    auto bulletObj = new GameObject(associated.GetLayer());
    auto bullet = new Bullet(*bulletObj,
                             angle,
                             PENGUIN_BULLET_SPEED,
                             PENGUIN_BULLET_DAMAGE,
                             PENGUIN_BULLET_DISTANCE,
                             "img/penguinbullet.png", 4, 0.1);

    bulletObj->AddComponent(bullet);

    auto cannonHole =  Vec2(associated.box.w/2, 0).RotateDeg(angle);

    bulletObj->SetCenter(associated.box.Center());
    bulletObj->box += cannonHole;
    Game::GetInstance().GetCurrentState().AddObject(bulletObj);
}

