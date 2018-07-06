//
// Created by fabio on 06/07/18.
//

#include <Sprite.h>
#include <Collider.h>
#include <CollisionMap.h>
#include "RollingStones.h"

RollingStones::RollingStones(GameObject &associated, int damage, float speed, float maxDist) : 
Component(associated), damage(damage), maxDist(maxDist), distCovered(0) {

    Sprite *spr = new Sprite(associated, ROLLINGROCKS_SPRITE, 10, 0.1);

    associated.AddComponent(spr);
    associated.box.h = spr->GetHeight();
    associated.box.w = spr->GetWidth();

    associated.SetCenter({associated.box.x, associated.box.y});

    auto collider = new Collider(associated);
    collider->SetCanCollide([] (GameObject &other) -> bool {
        return (other.HasComponent(PLAYER_TYPE) || other.HasComponent(COLLISION_MAP_TYPE));
    });

    associated.AddComponent(collider);
}

RollingStones::~RollingStones() {

}

void RollingStones::Update(float dt) {
    associated.box += speedVec;
    distCovered += speed;
    
    if(maxDist > 0 && distCovered >= maxDist){
        associated.RequestDelete();
    }
}

void RollingStones::Render() {

}

bool RollingStones::Is(string type) {
    return type == "RollingStones";
}

void RollingStones::Start() {
    auto stoneCenter = associated.box.Center();
    auto playerCenter = Player::player->GetCenter();
    auto d = playerCenter - stoneCenter;
    targetAngleDeg = d.XAngleDeg();
    speedVec = Vec2(speed*cos(M_PI*targetAngleDeg/180.0), speed*sin(M_PI*targetAngleDeg/180.0));
}

void RollingStones::NotifyCollision(GameObject &other) {
    if(other.HasComponent(PLAYER_TYPE)){
        Player::player->DecreaseHp(damage);
    } else if(other.HasComponent(COLLISION_MAP_TYPE)){
        associated.RequestDelete();
    }
}
