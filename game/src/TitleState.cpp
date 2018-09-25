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
#include <FadeEffect.h>
#include <Sound.h>
#include "TitleState.h"
#include "config.h"

TitleState::TitleState() : State(), frozen(false) {
    auto bgObj = new GameObject();
    bgObj->AddComponent(new Sprite(*bgObj, ASSETS_PATH("/img/bg.png") ));
    bgObj->AddComponent(new CameraFollower(*bgObj));
    AddObject(bgObj);

    bgMusic = new Music( ASSETS_PATH("/audio/menu.ogg") );
    bgMusic->Play();

    auto titleObj = new GameObject(1);
    SDL_Color white = {255, 255, 255, 255};
    auto titleText = new Text(*titleObj, ASSETS_PATH("/font/leadcoat.ttf"), 100, Text::TextStyle::SOLID, "NEPHESH", white);
    titleObj->AddComponent(titleText);
    titleObj->box += Vec2(20, 20);
    AddObject(titleObj);

    auto intrObj = new GameObject(1);
    auto instrText = new Text(*intrObj, ASSETS_PATH("/font/leadcoat.ttf"), 40, Text::TextStyle::SOLID, "Press space bar to start!", white);
    intrObj->AddComponent(instrText);
    auto callback = [instrText] {
        auto color = instrText->GetColor();
        color.a = (Uint8) (color.a == 255 ? 0 : 255);
        instrText->SetColor(color);
        instrText->RemakeTexture();
    };
    intrObj->AddComponent(new IntervalTimer(*intrObj, 0.6, callback));
    intrObj->box.x = GAME_WIDTH/2 - intrObj->box.w/2;
    intrObj->box.y = GAME_HEIGHT/2 + 400;
    AddObject(intrObj);

    auto fadeInObj = new GameObject(2);
    this->frozen = true;
    fadeInObj->AddComponent(new FadeEffect(*fadeInObj, TITLE_FADE_IN_DURATION, 1, [this] { this->frozen = false; }));
    AddObject(fadeInObj);
}

TitleState::~TitleState() = default;

void TitleState::Update(float dt) {
    auto inputManager = InputManager::GetInstance();

    auto isStart = inputManager.KeyPress(SPACE_KEY);
    if (!frozen && (isStart || inputManager.QuitRequested() || inputManager.KeyPress(ESCAPE_KEY))) {
        auto fadeObj = new GameObject(2);
        bgMusic->Stop();
        frozen = true;
        function<void()> callback;

        auto soundObject = new GameObject();
        auto sound = new Sound(*soundObject, isStart ? ASSETS_PATH("/audio/menu/select.wav") : ASSETS_PATH("/audio/menu/return.wav") );
        soundObject->AddComponent(sound);
        sound->Play();
        delete soundObject;

        if (isStart) {
            callback = [] {
                Game::GetInstance().Push(new WorldState());
            };
        } else {
            callback = [&] {
                quitRequested = true;
            };
        }

        fadeObj->AddComponent(new FadeEffect(*fadeObj, isStart ? TITLE_START_FADE_OUT_DURATION : TITLE_QUIT_FADE_OUT_DURATION, 0, callback, FadeEffect::FadeType::OUT));
        AddObject(fadeObj);
    }

    UpdateArray(dt);

    for (auto &it: objectLayers) {
        auto &objects = it.second;

        for(int i = 0; i < objects.size(); i++) {
            if (objects[i]->IsDead()) {
                objects.erase(objects.begin() + i);
                i--;
            }
        }
    }
}

void TitleState::Render() {
    RenderArray();
}

void TitleState::Start() {
    StartArray();
}

void TitleState::Pause() {

}

void TitleState::Resume() {
    auto fadeInObj = new GameObject(2);
    this->frozen = true;
    fadeInObj->AddComponent(new FadeEffect(*fadeInObj, 3, 1, [this] { this->frozen = false; }));
    AddObject(fadeInObj);
    Camera::pos = Vec2(0, 0);
    bgMusic->Play();
}

void TitleState::LoadAssets() {

}