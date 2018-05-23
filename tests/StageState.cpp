#include <Log.h>
#include <Game.h>
#include <InputManager.h>
#include <Camera.h>
#include <CameraFollower.h>
#include <algorithm>
#include <PenguinBody.h>
#include <Collider.h>
#include <StageState.h>
//#include <GameData.h>
//#include <Collision.h>
//#include <EndState.h>
//#include <Alien.h>


StageState::StageState() : backgroundMusic("tests/audio/stageState.ogg"),
                 tileMap(nullptr),
                 bg(nullptr) {

    Camera::SetCameraHeight(5);
    Camera::SetLayerDepth(1, 4.5);

    bg = new GameObject();
    bg->AddComponent(new Sprite(*bg, "tests/img/ocean.jpg"));
    bg->AddComponent(new CameraFollower(*bg));

    auto mapObject = new GameObject();
    mapObject->box.h = HEIGHT;
    mapObject->box.w = WIDTH;

    auto set = new TileSet(64, 64, "tests/img/tileset.png");
    tileMap = new TileMap(*mapObject, "tests/map/tileMap.txt", set);
    mapObject->AddComponent(tileMap);

//    Alien::alienCount = 0;
//
//    auto alienObject1 = new GameObject(1);
//    alienObject1->box = Rect(rand() % MAP_WIDTH, rand() % MAP_HEIGHT, 0, 0);
//    alienObject1->AddComponent(new Alien(*alienObject1, 5));
//    AddObject(alienObject1);
//
//    auto alienObject2 = new GameObject(1);
//    alienObject2->box = Rect(rand() % MAP_WIDTH, rand() % MAP_HEIGHT, 0, 0);
//    alienObject2->AddComponent(new Alien(*alienObject2, 5, 2*ALIEN_COOLDOWN));
//    AddObject(alienObject2);
//
//    auto alienObject3 = new GameObject(1);
//    alienObject3->box = Rect(rand() % MAP_WIDTH, rand() % MAP_HEIGHT, 0, 0);
//    alienObject3->AddComponent(new Alien(*alienObject3, 5, 3*ALIEN_COOLDOWN));
//    AddObject(alienObject3);

    auto playerObject = new GameObject();
    playerObject->box = Rect(704, 640);
    playerObject->AddComponent(new PenguinBody(*playerObject));
    AddObject(playerObject);

    Camera::Follow(playerObject);

    backgroundMusic.Play();
}

void StageState::LoadAssets() {
}

void StageState::Update(float dt) {
    auto inputManager = InputManager::GetInstance();

    Camera::Update(dt);
    bg->Update(dt);

    quitRequested = inputManager.QuitRequested();

    popRequested = inputManager.KeyPress(ESCAPE_KEY);

    UpdateArray(dt);

      // The active part of a collision
//    auto colliders = getColliders();
      // The passive part of a collision
//    auto collidables = getCollidables();
//
//    for (auto &collider : colliders) {
//        vector<GameObject> collidables;
//        colliderCpt = collider.getComponent(COLLIDER_TYPE);
//        for (auto &collidable : collidables) {
//            collidableCpt = collidable.getComponent(COLLIDABLE_TYPE);
//            if (collidableCpt.IsColliding(colliderCpt)) {
//                collider.notifyColision(collidable);
//            }
//        }
//    }


//    for (auto &it: objectLayers) {
//        auto &objects = it.second;
//        auto colliderArray = new Collider*[objects.size()];
//        memset(colliderArray, 0, objects.size()*sizeof(Collider *));
//        for (int i = 0; i < objects.size(); ++i) {
//            for (int j = i; j < objects.size(); ++j) {
//                if (i == 0) {
//                    colliderArray[j] = (Collider *)(*objects[j]).GetComponent(COLLIDER_TYPE);
//                }
//                if (i != 0 && colliderArray[i] == nullptr) {
//                    break;
//                }
//
//                if (i != j && colliderArray[j] != nullptr && colliderArray[i] != nullptr) {
//                    auto angleIRad = 2*M_PI*((*objects[i]).angleDeg/360);
//                    auto angleJRad = 2*M_PI*((*objects[j]).angleDeg/360);
//                    auto boxI = colliderArray[i]->box;
//                    auto boxJ = colliderArray[j]->box;
//
//                    if (Collision::IsColliding(boxI, boxJ, angleIRad, angleJRad)) {
//                        (*objects[i]).NotifyCollision((*objects[j]));
//                        (*objects[j]).NotifyCollision((*objects[i]));
//                    }
//                }
//            }
//        }
//    }

    for (auto &it: objectLayers) {
        auto &objects = it.second;

        for(int i = 0; i < objects.size(); i++) {
            if (objects[i]->IsDead()) {
                objects.erase(objects.begin() + i);
            }
        }
    }
//    auto endGame = Alien::alienCount == 0 || PenguinBody::player == nullptr;
//    if (endGame) {
//        GameData::playerVictory = PenguinBody::player != nullptr;
//        Game::GetInstance().Push(new EndState());
//    }
}

void StageState::Render() {
    bg->Render();
    for (int i = 0; i < tileMap->GetDepth(); i++) {
        auto it = objectLayers.find(i);
        tileMap->RenderLayer(i);

        if (it != objectLayers.end()) {
            auto &objects = (*it).second;

            for (auto &object : objects) {
                object->Render();
            }
        }
    }
}

StageState::~StageState() {
    objectLayers.clear();
}

void StageState::Start() {
    StartArray();
}

void StageState::Pause() {

}

void StageState::Resume() {

}

