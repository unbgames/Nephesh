//
// Created by ftfnunes on 19/05/18.
//

#include <Sprite.h>
#include <Collider.h>
#include <LineSegment.h>
#include "BeamSkill.h"

BeamSkill::BeamSkill(GameObject &associated, Vec2 target) : Component(associated), target(target) {
    auto raySprite = new Sprite(associated, "img/ray.png");
    associated.AddComponent(raySprite);
    auto collider = new Collider(associated);
    associated.AddComponent(collider);
}

BeamSkill::~BeamSkill() {

}

void BeamSkill::Update(float dt) {

}

void BeamSkill::Render() {

}

bool BeamSkill::Is(string type) {
    return type == BEAM_SKILL_TYPE;
}

void BeamSkill::NotifyCollision(GameObject &other) {
    auto otherCollider = (Collider *) other.GetComponent(COLLIDER_TYPE);
    auto thisCollider = (Collider *) associated.GetComponent(COLLIDER_TYPE);
    auto colliderBox = thisCollider->box;

    auto intersections = thisCollider->GetIntersections(*otherCollider);
    vector<LineSegment> laterals;
    laterals.emplace_back(Vec2(colliderBox.x, colliderBox.y), Vec2(colliderBox.x+colliderBox.w, colliderBox.y));
    laterals.emplace_back(Vec2(colliderBox.x, colliderBox.y+colliderBox.h), Vec2(colliderBox.x+colliderBox.w, colliderBox.y+colliderBox.h));
    for (auto &intersection : intersections) {
        auto intersectionDot = intersection.second;
        auto intersectionLine = intersection.first;

        for (auto &lateral : laterals) {
            if (lateral == intersectionLine) {
                auto d = (intersectionDot - lateral.A).Module();
                if (d < cutoffPoint) {
                    cutoffPoint = d;
                }
            }
        }
    }

    thisCollider->box.w = cutoffPoint;
}

void BeamSkill::Start() {
    auto collider = (Collider *) associated.GetComponent(COLLIDER_TYPE);
    auto d = target - Vec2(associated.box.x, associated.box.y);
    cout << d.XAngleDeg() << endl;
    associated.angleDeg = d.XAngleDeg();
//    cutoffPoint = d.Module();
//    collider->box.w = cutoffPoint;
}
