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
#include <Player.h>
#include <Collidable.h>
#include <TextBox.h>
#include "TitleState.h"
#include "Text.h"

TitleState::TitleState() : State() {
    auto bgObj = new GameObject();
    bgObj->AddComponent(new Sprite(*bgObj, "img/whiteBg.png"));
    bgObj->AddComponent(new CameraFollower(*bgObj));
    AddObject(bgObj);

    auto playerObj = new GameObject();
    playerObj->box.x = WIDTH/2;
    playerObj->box.y = HEIGHT/2;
    playerObj->AddComponent(new Player(*playerObj));
    AddObject(playerObj);

    auto textBox = new GameObject(1);
    auto box = new TextBox(*textBox);
    textBox->AddComponent(box);
    AddObject(textBox);
}

TitleState::~TitleState() = default;

void TitleState::Update(float dt) {
    auto& inputManager = InputManager::GetInstance();

    if (inputManager.KeyPress(SDLK_t)) {
        auto mousePos = inputManager.GetMouse();

        auto blockObj = new GameObject();
        blockObj->angleDeg = 45;
        blockObj->AddComponent(new Sprite(*blockObj, "img/block.png"));
        blockObj->AddComponent(new Collidable(*blockObj));
        blockObj->SetCenter(Camera::pos +  mousePos);
        AddObject(blockObj);
    }

//    if (inputManager.MousePress(LEFT_MOUSE_BUTTON)) {
//        auto target = Vec2(inputManager.GetMouseX(), inputManager.GetMouseY());
//        auto beamObj = new GameObject(1);
//        beamObj->box.x = WIDTH/2;
//        beamObj->box.y = HEIGHT/2;
//        auto beamCpt = new BeamSkill(*beamObj, target);
//        beamObj->AddComponent(beamCpt);
//        this->AddObject(beamObj);
//    }
    
    UpdateArray(dt);

    CheckCollisions();

    for (auto &it: objectLayers) {
        auto &objects = it.second;

        for(int i = 0; i < objects.size(); i++) {
            if (objects[i]->IsDead()) {
                objects.erase(objects.begin() + i);
                i--;
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

weak_ptr<GameObject> TitleState::AddCollidable(shared_ptr<GameObject> object) {
    collidables.emplace_back(object);
    return weak_ptr<GameObject>(object);
}

weak_ptr<GameObject> TitleState::AddCollider(shared_ptr<GameObject> object) {
    colliders.emplace_back(object);
    return weak_ptr<GameObject>(object);
}

void TitleState::CheckCollisions() {
    for (int i = 0; i < colliders.size(); i++) {
        if (auto collider = colliders[i].lock()) {
            auto colliderCpt = (Collider*)collider->GetComponent(COLLIDER_TYPE);

            for (int j = 0; j < collidables.size(); j++) {
                auto collidable = collidables[j].lock();

                if( (collidable) && (collider->GetLayer() == collidable->GetLayer()) && (colliderCpt->CanCollide(*collidable)) ){
                    auto collidableCpt = (Collidable*)collidable->GetComponent(COLLIDABLE_TYPE);

                    if(collidableCpt->IsColliding(*colliderCpt)){
                        collider->NotifyCollision(*collidable);
                    }
                }
                else if(!collidable){
                    collidables.erase(collidables.begin() + j);
                    j--;
                }
            }
        }
        else{
            colliders.erase(colliders.begin() + i);
            i--;
        }
    }
}

weak_ptr<GameObject> TitleState::AddObject(shared_ptr<GameObject> object) {
    auto collidable = (Collidable*) object->GetComponent(COLLIDABLE_TYPE);
    auto collider = (Collider*) object->GetComponent(COLLIDER_TYPE);

    if(collidable){
        AddCollidable(object);
    }

    if(collider){
        AddCollider(object);
    }

    return State::AddObject(object);
}

weak_ptr<GameObject> TitleState::AddObject(GameObject *object) {
    auto ptr = shared_ptr<GameObject>(object);

    return AddObject(ptr);
}
