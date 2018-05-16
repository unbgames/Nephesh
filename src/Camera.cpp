//
// Created by ftfnunes on 29/03/18.
//

#include <InputManager.h>
#include <Game.h>
#include "Camera.h"

Vec2 Camera::pos = Vec2();
Vec2 Camera::speed = Vec2();
GameObject *Camera::focus = nullptr;
unordered_map<int, float> Camera::layerHeights;
float Camera::cameraHeight = 0;


void Camera::Follow(GameObject *newFocus) {
    focus = newFocus;
}

void Camera::Unfollow() {
    focus = nullptr;
}

void Camera::Update(float dt) {
    if (focus != nullptr) {
        auto center = focus->box.Center();
        pos = Vec2(center.x - WIDTH/2, center.y - HEIGHT/2);
    } else {
        auto inputManager = InputManager::GetInstance();

        speed = Vec2();
        if (inputManager.IsKeyDown(LEFT_ARROW_KEY)) {
            speed = Vec2(-CAMERA_SPEED*dt, 0);
        } else if (inputManager.IsKeyDown(DOWN_ARROW_KEY)) {
            speed = Vec2(0, CAMERA_SPEED*dt);
        } else if (inputManager.IsKeyDown(UP_ARROW_KEY)) {
            speed = Vec2(0, -CAMERA_SPEED*dt);
        } else if (inputManager.IsKeyDown(RIGHT_ARROW_KEY)) {
            speed = Vec2(CAMERA_SPEED*dt, 0);
        }
        pos += speed;
    }
}

void Camera::SetLayerHeight(int layer, float height) {
    layerHeights[layer] = height;
}

float Camera::GetLayerScale(int layer) {
    auto height = layerHeights.find(layer);

    if (height != layerHeights.end()) {
        return cameraHeight/((*height).second);
    }

    return 1.0;
}

void Camera::SetCameraHeight(float height) {
    cameraHeight = height;
}

Vec2 Camera::GetRenderPosition(int layer, Vec2 absolutePosition) {
    return GetRenderPosition(absolutePosition, GetLayerScale(layer));
}

Vec2 Camera::GetRenderPosition(Vec2 absPosition, float layerScale) {
    auto center = Vec2(WIDTH/2, HEIGHT/2);
    return (absPosition - center - pos)*layerScale + center;
}

Vec2 Camera::GetClickPosition(int layer, Vec2 mouseClick, bool correctCamera) {
    auto center = Vec2(WIDTH/2, HEIGHT/2);
    auto height = layerHeights.find(layer);
    auto inverseScale = 1.0;

    if (height != layerHeights.end()) {
        inverseScale = ((*height).second)/cameraHeight;
    }

    return (mouseClick - center)*inverseScale + center + (correctCamera ? pos : Vec2());
}

bool Camera::IsFollowing() {
    return focus != nullptr;
}

