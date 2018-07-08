//
// Created by ftfnunes on 08/07/18.
//

#include <InputManager.h>
#include "Debug.h"

Debug::Debug(GameObject &associated, Vec2 offset) : Component(associated), offset(offset) {

}

void Debug::Update(float dt) {
    auto &inputManager = InputManager::GetInstance();

    if (inputManager.IsKeyDown(UP_ARROW_KEY)) {
        associated.box.y -= 1;
    }
    if (inputManager.IsKeyDown(DOWN_ARROW_KEY)) {
        associated.box.y += 1;
    }
    if (inputManager.IsKeyDown(LEFT_ARROW_KEY)) {
        associated.box.x -= 1;
    }
    if (inputManager.IsKeyDown(RIGHT_ARROW_KEY)) {
        associated.box.x += 1;
    }

    if (inputManager.KeyPress(SDLK_p)) {
        cout << "X: " << associated.box.x - offset.x << ", Y: " << associated.box.y - offset.y << endl;
    }
}

void Debug::Render() {

}

bool Debug::Is(string type) {
    return false;
}
