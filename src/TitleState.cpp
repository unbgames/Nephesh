#include <Sprite.h>
#include <InputManager.h>
#include <Game.h>
#include <CameraFollower.h>
#include <Camera.h>
#include <IntervalTimer.h>
#include <tiff.h>
#include <TileSet.h>
#include <BeamSkill.h>
#include <Collider.h>
#include <Collision.h>
#include "TitleState.h"
#include "Text.h"

TitleState::TitleState() : State() {
    auto bgObj = new GameObject();
    bgObj->AddComponent(new Sprite(*bgObj, "img/bg.png"));
    bgObj->AddComponent(new CameraFollower(*bgObj));
    AddObject(bgObj);

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
        color.a = (uint8) (color.a == 255 ? 0 : 255);
        instrText->SetColor(color);
        instrText->RemakeTexture();
    };
    intrObj->AddComponent(new IntervalTimer(*intrObj, 0.6, callback));
    intrObj->box.x = WIDTH/2 - intrObj->box.w/2;
    intrObj->box.y = HEIGHT/2 + 200;
    AddObject(intrObj);
}

TitleState::~TitleState() = default;

void TitleState::Update(float dt) {
    auto &inputManager = InputManager::GetInstance();

    if (inputManager.MousePress(RIGHT_MOUSE_BUTTON)) {
        auto mousePos = inputManager.GetMouse();

        auto blockObj = new GameObject(1);
        blockObj->angleDeg = 45;
        blockObj->AddComponent(new Sprite(*blockObj, "img/block.png"));
        blockObj->AddComponent(new Collider(*blockObj));
        blockObj->SetCenter(Camera::pos +  mousePos);
        AddObject(blockObj);
    }

    if (inputManager.MousePress(LEFT_MOUSE_BUTTON)) {
        auto target = Vec2(inputManager.GetMouseX(), inputManager.GetMouseY());
        auto beamObj = new GameObject(1);
        beamObj->box.x = WIDTH/2;
        beamObj->box.y = HEIGHT/2;
        auto beamCpt = new BeamSkill(*beamObj, target);
        beamObj->AddComponent(beamCpt);
        this->AddObject(beamObj);
    }
    
    UpdateArray(dt);

    for (auto &it: objectLayers) {
        auto &objects = it.second;
        auto colliderArray = new Collider*[objects.size()];
        memset(colliderArray, 0, objects.size()*sizeof(Collider *));
        for (int i = 0; i < objects.size(); ++i) {
            for (int j = i; j < objects.size(); ++j) {
                if (i == 0) {
                    colliderArray[j] = (Collider *)(*objects[j]).GetComponent(COLLIDER_TYPE);
                }
                if (i != 0 && colliderArray[i] == nullptr) {
                    break;
                }

                if (i != j && colliderArray[j] != nullptr && colliderArray[i] != nullptr) {
                    if (Collision::IsColliding(*colliderArray[i], *colliderArray[j])) {
                        (*objects[i]).NotifyCollision((*objects[j]));
                        (*objects[j]).NotifyCollision((*objects[i]));
                    }
                }
            }
        }
    }

    for (auto &it: objectLayers) {
        auto &objects = it.second;

        for(int i = 0; i < objects.size(); i++) {
            if (objects[i]->IsDead()) {
                objects.erase(objects.begin() + i);
            }
        }
    }

    quitRequested = inputManager.QuitRequested() || inputManager.KeyPress(ESCAPE_KEY);
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
    Camera::pos = Vec2(0, 0);
}

void TitleState::LoadAssets() {

}
