#include <Log.h>
#include <Game.h>
#include <InputManager.h>
#include <Camera.h>
#include <CameraFollower.h>
#include <algorithm>
#include <memory>
#include <PenguinBody.h>
#include <Collider.h>
#include <StageState.h>
//#include <GameData.h>
//#include <Collision.h>
//#include <EndState.h>
//#include <Alien.h>


StageState::StageState() : backgroundMusic("tests/audio/stageState.ogg"),
                 bg(nullptr) {

    Camera::SetCameraHeight(5);
    Camera::SetLayerDepth(1, 4.5);

    bg = shared_ptr<GameObject>(new GameObject());
    bg->AddComponent(new Sprite(*bg, "tests/img/ocean.jpg"));
    bg->AddComponent(new CameraFollower(*bg));

    map = shared_ptr<GameObject>(new GameObject());
    map->box.h = HEIGHT;
    map->box.w = WIDTH;

    auto set = new TileSet(64, 64, "tests/img/tileset.png");
    auto tileMap = new TileMap(*map, "tests/map/tileMap.txt", set);
    auto collisionMap = new CollisionMap(*map, "tests/map/collisionMap.txt", 64, 64);
    map->AddComponent(tileMap);
    map->AddComponent(collisionMap);
    AddCollidable(map);

    auto playerObject = new GameObject();
    playerObject->AddComponent(new PenguinBody(*playerObject));
    AddObject(playerObject);

    for(int i = 0; i < 100; i++ ){
        auto testSprite = new GameObject();
        testSprite->AddComponent(new Sprite(*testSprite, "tests/img/alien.png"));
        testSprite->AddComponent(new Collider(*testSprite));
        testSprite->box.PlaceCenterAt({500 + (rand() % 200), 500 + (rand() % 200)});
        AddObject(testSprite);
    }

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

    for (auto &it: objectLayers) {
        auto &objects = it.second;

        for(int i = 0; i < objects.size(); i++) {
            if (objects[i]->IsDead()) {
                objects.erase(objects.begin() + i);
            }
        }
    }

    CheckCollisions();
}

void StageState::Render() {
    bg->Render();
    auto tileMap = (TileMap*) map->GetComponent(TILE_MAP_TYPE);

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

void StageState::CheckCollisions() {


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

    auto begin = clock();

    for (int i = 0; i < colliders.size(); i++) {
        if (auto collider = colliders[i].lock()) {
            auto colliderCpt = (Collider*)collider->GetComponent(COLLIDER_TYPE);
            for (int j = 0; j < collidables.size(); j++) {
                auto collidable = collidables[j].lock();
                if(collidable && collider->GetLayer() == collidable->GetLayer()){
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

    cout << double(clock() - begin)*1000/CLOCKS_PER_SEC << " ms" << endl;
}

weak_ptr<GameObject> StageState::AddObject(GameObject *object) {
    auto ptr = shared_ptr<GameObject>(object);

    auto collidable = (Collidable*) object->GetComponent(COLLIDABLE_TYPE);
    auto collider = (Collider*) object->GetComponent(COLLIDER_TYPE);

    if(collidable){
        AddCollidable(ptr);
    }

    if(collider){
        AddCollider(ptr);
    }

    return State::AddObject(ptr);
}

weak_ptr<GameObject> StageState::AddCollidable(shared_ptr<GameObject> object) {
    collidables.emplace_back(object);
    return weak_ptr<GameObject>(object);
}

weak_ptr<GameObject> StageState::AddCollider(shared_ptr<GameObject> object) {
    colliders.emplace_back(object);
    return weak_ptr<GameObject>(object);
}

