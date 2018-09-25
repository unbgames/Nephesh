//
// Created by ftfnunes on 29/03/18.
//

#include <cstring>
#include <Camera.h>
#include "InputManager.h"

InputManager::InputManager() : quitRequested(false), updateCounter(0), mouseX(0), mouseY(0)  {
    memset(mouseState, 0, NUM_MOUSE_BUTTONS* sizeof(bool));
    memset(mouseUpdate, 0, NUM_MOUSE_BUTTONS* sizeof(int));
}

void InputManager::Update() {
    SDL_Event event;

    SDL_GetMouseState(&mouseX, &mouseY);
    quitRequested = false;
    updateCounter++;

    while (SDL_PollEvent(&event)) {
        if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0) {
            keyState[event.key.keysym.sym] = event.type == SDL_KEYDOWN;
            keyUpdate[event.key.keysym.sym] = updateCounter;
        } else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
            mouseState[event.button.button] = event.type == SDL_MOUSEBUTTONDOWN;
            mouseUpdate[event.button.button] = updateCounter;
        } else if (event.type == SDL_QUIT) {
            quitRequested = true;
        }
    }
}

bool InputManager::KeyPress(int key) {
    return keyState[key] && keyUpdate[key] == updateCounter;
}

bool InputManager::KeyRelease(int key) {
    return !keyState[key] && keyUpdate[key] == updateCounter;
}

bool InputManager::IsKeyDown(int key) {
    return keyState[key];
}

bool InputManager::MousePress(int button) {
    return mouseState[button] && mouseUpdate[button] == updateCounter;
}

bool InputManager::MouseRelease(int button) {
    return !mouseState[button] && mouseUpdate[button] == updateCounter;
}

bool InputManager::IsMouseDown(int button) {
    return mouseState[button];
}

int InputManager::GetMouseX() {
    return mouseX;
}

int InputManager::GetMouseY() {
    return mouseY;
}

bool InputManager::QuitRequested() {
    return quitRequested;
}

InputManager &InputManager::GetInstance() {
    static InputManager manager;

    return manager;
}

Vec2 InputManager::GetMouse() {
    return Vec2(mouseX, mouseY);
}

