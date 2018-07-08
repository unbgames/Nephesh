//
// Created by ftfnunes on 29/03/18.
//

#include <InputManager.h>
#include <Game.h>
#include <Collider.h>
#include <WorldState.h>
#include "Camera.h"

Vec2 Camera::pos = Vec2();
Vec2 Camera::speed = Vec2();
GameObject *Camera::focus = nullptr;
unordered_map<int, float> Camera::layerDepths;
float Camera::cameraHeight = 0;


void Camera::Follow(GameObject *newFocus) {
    focus = newFocus;
}

void Camera::Unfollow() {
    focus = nullptr;
}

void Camera::Update(float dt) {
    if (focus != nullptr) {
        auto collider = (Collider *) focus->GetComponent(COLLIDER_TYPE);
        Vec2 center;
        if (collider != nullptr) {
            center = collider->box.Center();
        } else {
            center = focus->box.Center();
        }
        pos = Vec2(center.x - WIDTH/2, center.y - HEIGHT/2);

        auto &state = (WorldState &) Game::GetInstance().GetCurrentState();
        auto &currentMap = state.GetCurrentMap();
        auto currentTileMap = currentMap.GetTileMap();
        auto &nextMap = state.GetNextMap();
        auto &prevMap = state.GetPreviousMap();
        auto mapMinX = currentTileMap->box.x;
        auto mapMaxX = currentTileMap->box.x + currentTileMap->box.w;
        auto mapMinY = currentTileMap->box.y;
        auto mapMaxY = currentTileMap->box.y + currentTileMap->box.h;

        if (pos.x < mapMinX && nextMap.GetDirection() != Map::LEFT && (&prevMap == &currentMap || currentMap.GetDirection() != Map::RIGHT)) {
            pos.x = mapMinX;
        } else if (pos.x+WIDTH > mapMaxX && nextMap.GetDirection() != Map::RIGHT && (&prevMap == &currentMap || currentMap.GetDirection() != Map::LEFT)) {
            pos.x = mapMaxX-WIDTH;
        }

        if (pos.y < mapMinY && nextMap.GetDirection() != Map::UP && (&prevMap == &currentMap || currentMap.GetDirection() != Map::DOWN)) {
            pos.y = mapMinY;
        } else if (pos.y+HEIGHT > mapMaxY && nextMap.GetDirection() != Map::DOWN && (&prevMap == &currentMap || currentMap.GetDirection() != Map::UP)) {
            pos.y = mapMaxY-HEIGHT;
        }

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

// Depth: vertical distance between camera and layer
void Camera::SetLayerDepth(int layer, float depth) {
    layerDepths[layer] = depth;
}

float Camera::GetLayerScale(int layer) {
    auto height = layerDepths.find(layer);

    if (height != layerDepths.end()) {
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

Vec2 Camera::GetAbsolutePosition(int layer, Vec2 mouseClick, bool correctCamera) {
    auto center = Vec2(WIDTH/2, HEIGHT/2);
    auto height = layerDepths.find(layer);
    auto inverseScale = 1.0;

    if (height != layerDepths.end()) {
        inverseScale = ((*height).second)/cameraHeight;
    }

    return (mouseClick - center)*inverseScale + center + (correctCamera ? pos : Vec2());
}

bool Camera::IsFollowing() {
    return focus != nullptr;
}

