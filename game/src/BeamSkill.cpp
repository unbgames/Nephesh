//
// Created by ftfnunes on 19/05/18.
//

#include <Sprite.h>
#include <Collider.h>
#include <LineSegment.h>
#include <Game.h>
#include <Collidable.h>
#include "BeamSkill.h"
#include "config.h"

BeamSkill::BeamSkill(GameObject &associated, Vec2 target, Player::PlayerDirection direction) : Component(associated), target(target), direction(direction), lockBeam(false) {
    auto collider = new Collider(associated);
    associated.AddComponent(collider);
    auto raySprite = new Sprite(associated, ASSETS_PATH("/img/magic_effect_side2.png"), 5, BEAM_LIFETIME/5, 0, false, false);
    associated.AddComponent(raySprite);
    collider->SetCanCollide([] (GameObject &other) -> bool {
        return false;
    });
}

BeamSkill::~BeamSkill() {
}

void BeamSkill::Update(float dt) {
    timer.Update(dt);
    if (timer.Get() > BEAM_LIFETIME) {
        timer.Restart();
        initObject.lock()->RequestDelete();
        endObject.lock()->RequestDelete();
        associated.RequestDelete();
    }
}

void BeamSkill::Render() {
    lockBeam = true;
}

bool BeamSkill::Is(string type) {
    return type == BEAM_SKILL_TYPE;
}

void BeamSkill::NotifyCollision(GameObject &other) {
    if (!lockBeam) {
        auto collidable = (Collidable *) other.GetComponent(COLLIDABLE_TYPE);
        auto thisCollider = (Collider *) associated.GetComponent(COLLIDER_TYPE);
        auto colliderBox = thisCollider->box;

        auto intersections = collidable->GetIntersections(*thisCollider);
        auto boxCorners = colliderBox.GetCorners(associated.angleDeg, associated.rotationCenter);
        auto l1 = LineSegment(boxCorners[0], boxCorners[1]);
        auto l2 = LineSegment(boxCorners[2], boxCorners[3]);
        for (auto &intersection : intersections) {
            auto intersectionDot = intersection.intersectionPoint;
            auto intersectionLine = intersection.colliderLine;

            if (intersectionLine == l1) {
                auto d = (intersectionDot - l1.dot1).Module();
                if (d < cutoffPoint) {
                    cutoffPoint = d;
                }
            } else if (intersectionLine == l2) {
                auto d = (intersectionDot - l2.dot2).Module();
                if (d < cutoffPoint) {
                    cutoffPoint = d;
                }
            }
        }

        this->associated.box.w = cutoffPoint;
        thisCollider->box.w = cutoffPoint;
        auto sprite = (Sprite *) associated.GetComponent(SPRITE_TYPE);
        auto clip = sprite->GetClip();
        sprite->SetClip(clip.x, clip.y, cutoffPoint, clip.h);

        endObject.lock()->SetCenter((Vec2(associated.box.x + cutoffPoint, associated.box.y + associated.box.h/2) - Vec2(associated.box.x, associated.box.y + associated.box.h/2)).RotateDeg(associated.angleDeg) + Vec2(associated.box.x, associated.box.y + associated.box.h/2));
    }
}

void BeamSkill::Start() {
    auto sprite = (Sprite *) associated.GetComponent(SPRITE_TYPE);

    if (direction == Player::PlayerDirection::UP) {
        associated.box += Vec2(24, 0);
    } else if (direction == Player::PlayerDirection::DOWN) {
        associated.box += Vec2(17, 0);
    }

    auto collider = (Collider *) associated.GetComponent(COLLIDER_TYPE);
    collider->SetCanCollide([] (GameObject &other) -> bool {
        return true;
    });
    collider->Update(0);

    auto d = target - Vec2(associated.box.x, associated.box.y + associated.box.h/2);
    associated.rotationCenter = Vec2(0, associated.box.h/2);
    associated.angleDeg = d.XAngleDeg();
    cutoffPoint = associated.box.w;
    auto clip = sprite->GetClip();
    sprite->SetClip(clip.x, clip.y, cutoffPoint, clip.h);

    auto initObj = new GameObject(associated.GetLayer());
    initObj->angleDeg = associated.angleDeg;
    initObj->AddComponent(new Sprite(*initObj, ASSETS_PATH("/img/magic_effect_side1.png"), 5, BEAM_LIFETIME/5));
    initObj->SetCenter(Vec2(associated.box.x, associated.box.y + associated.box.h/2));
    initObject = Game::GetInstance().GetCurrentState().AddObject(initObj);

    auto endObj = new GameObject(associated.GetLayer());
    endObj->angleDeg = associated.angleDeg;
    auto s = new Sprite(*endObj, ASSETS_PATH("/img/magic_effect_side3.png"), 5, BEAM_LIFETIME/5);
    endObj->AddComponent(s);
    endObj->SetCenter(Vec2(associated.box.x, associated.box.y) + Vec2(associated.box.w, 0).RotateDeg(associated.angleDeg));
    endObject = Game::GetInstance().GetCurrentState().AddObject(endObj);
}

void BeamSkill::Fire() {

}
