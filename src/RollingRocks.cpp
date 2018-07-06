//
// Created by fabio on 06/07/18.
//

#include <Sprite.h>
#include "RollingRocks.h"

RollingRocks::RollingRocks(GameObject &associated, int damage) : Component
(associated), damage(damage) {

    Sprite *spr = new Sprite(associated, ROLLINGROCKS_SPRITE, 10, 0.1);

    associated.AddComponent(spr);
    associated.box.h = spr->GetHeight();
    associated.box.w = spr->GetWidth();

    associated.SetCenter({associated.box.x, associated.box.y});

}

RollingRocks::~RollingRocks() {

}

void RollingRocks::Update(float dt) {

}

void RollingRocks::Render() {

}

bool RollingRocks::Is(string type) {
    return false;
}

void RollingRocks::Start() {
    Component::Start();
}

void RollingRocks::NotifyCollision(GameObject &other) {
    Component::NotifyCollision(other);
}
