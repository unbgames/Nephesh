//
// Created by ftfnunes on 19/05/18.
//

#include <Sprite.h>
#include "BeamSkill.h"

BeamSkill::BeamSkill(GameObject &associated, Vec2 target) : Component(associated), target(target) {
//    auto raySprite = new Sprite(associated, "Ray.png");
//    associated.AddComponent(raySprite);
}

BeamSkill::~BeamSkill() {

}

void BeamSkill::Update(float dt) {

}

void BeamSkill::Render() {

}

bool BeamSkill::Is(string type) {
    return false;
}

void BeamSkill::NotifyCollision(GameObject &other) {

}

void BeamSkill::Start() {
    associated.angleDeg = (target - Vec2(associated.box.x, associated.box.y)).XAngleDeg();
}
