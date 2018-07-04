//
// Created by ftfnunes on 30/06/18.
//

#include <Camera.h>
#include <InputManager.h>
#include <IntervalTimer.h>
#include <Text.h>
#include <Sprite.h>
#include <CameraFollower.h>
#include <Game.h>
#include <WorldState.h>
#include "TitleState.h"

TitleState::TitleState() : State() {
    auto bgObj = new GameObject();
    bgObj->AddComponent(new Sprite(*bgObj, "img/bg.png"));
    bgObj->AddComponent(new CameraFollower(*bgObj));
    AddObject(bgObj);

    bgMusic = new Music("audio/menu.ogg");
    bgMusic->Play();

    auto titleObj = new GameObject(1);
    SDL_Color white = {255, 255, 255, 255};
    auto titleText = new Text(*titleObj, "font/leadcoat.ttf", 100, Text::TextStyle::SOLID, "NEPHESH", white);
    titleObj->AddComponent(titleText);
    titleObj->box += Vec2(20, 20);
    AddObject(titleObj);

    auto intrObj = new GameObject(1);
    auto instrText = new Text(*intrObj, "font/leadcoat.ttf", 40, Text::TextStyle::SOLID, "Press space bar to start!", white);
    intrObj->AddComponent(instrText);
    auto callback = [instrText] {
        auto color = instrText->GetColor();
        color.a = (Uint8) (color.a == 255 ? 0 : 255);
        instrText->SetColor(color);
        instrText->RemakeTexture();
    };
    intrObj->AddComponent(new IntervalTimer(*intrObj, 0.6, callback));
    intrObj->box.x = WIDTH/2 - intrObj->box.w/2;
    intrObj->box.y = HEIGHT/2 + 400;
    AddObject(intrObj);
}

TitleState::~TitleState() = default;

void TitleState::Update(float dt) {
    auto inputManager = InputManager::GetInstance();
    
    if (inputManager.KeyPress(SPACE_KEY)) {
        Game::GetInstance().Push(new WorldState());
    }
    
    UpdateArray(dt);

    quitRequested = inputManager.QuitRequested() || inputManager.KeyPress(ESCAPE_KEY);
}

void TitleState::Render() {
    RenderArray();
}

void TitleState::Start() {
    StartArray();
}

void TitleState::Pause() {
    bgMusic->Stop();
}

void TitleState::Resume() {
    Camera::pos = Vec2(0, 0);
    bgMusic->Play();
}

void TitleState::LoadAssets() {

}