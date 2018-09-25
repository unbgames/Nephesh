//
// Created by ftfnunes on 30/03/18.
//

#include <Camera.h>
#include "CameraFollower.h"


CameraFollower::CameraFollower(GameObject &associated, Vec2 position) : Component(associated), position(position) {}

void CameraFollower::Update(float dt) {
    associated.box.x = Camera::pos.x;
    associated.box.y = Camera::pos.y;

    associated.box += position;
}

void CameraFollower::Render() {}

bool CameraFollower::Is(string type) {
    return type == CAMERA_FOLLOWER_TYPE;
}

void CameraFollower::SetPosition(Vec2 position) {
    this->position = position;
}

