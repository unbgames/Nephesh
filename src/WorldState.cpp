#include <Sprite.h>
#include <InputManager.h>
#include <Game.h>
#include <CameraFollower.h>
#include <Camera.h>
#include <IntervalTimer.h>
#include <TileSet.h>
#include <BeamSkill.h>
#include <Collider.h>
#include <Player.h>
#include <Collidable.h>
#include <TextBox.h>
#include <Npc.h>
#include <Sound.h>
#include <FadeEffect.h>
#include "WorldState.h"
#include "Text.h"

WorldState::WorldState() : State(), currentMapIndex(0) {
    auto obj = new GameObject();
    bgObj = shared_ptr<GameObject>(obj);
    bgObj->AddComponent(new Sprite(*bgObj, "img/whiteBg.png"));
    bgObj->AddComponent(new CameraFollower(*bgObj));

    auto playerObj = new GameObject();
    playerObj->box.x = 1024;
    playerObj->box.y = 1024;
    playerObj->AddComponent(new Player(*playerObj));
    AddObject(playerObj);

    Camera::Follow(playerObj);

    bgMusic = new Music("audio/mundo.ogg");
    Mix_VolumeMusic(32);

    auto npcObj = new GameObject();
    npcObj->box.x = WIDTH/2;
    npcObj->box.y = HEIGHT/2;
    npcObj->AddComponent(new Sprite(*npcObj, "img/criatura.png", 6, 0.1));
    npcObj->AddComponent(new Npc(*npcObj, "npcs/npcTest.txt"));
    npcObj->AddComponent(new Sound(*npcObj, "audio/npcs/criatura_magica_1.wav"));

    AddObject(npcObj);
    Player::player->Freeze();
    auto fadeInObj = new GameObject(2);
    fadeInObj->AddComponent(new FadeEffect(*fadeInObj, 11.5, 2, [] { Player::player->Unfreeze(); }));
    AddObject(fadeInObj);
}

WorldState::~WorldState() = default;

void WorldState::Update(float dt) {
    bgObj->Update(dt);
    Camera::Update(dt);
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

    if (inputManager.KeyPress(ESCAPE_KEY)) {
        auto fadeObj = new GameObject(2);
        bgMusic->Stop();
        function<void()> callback;
        callback = [&] {
            popRequested = true;
        };

        fadeObj->AddComponent(new FadeEffect(*fadeObj, 1, 0, callback, FadeEffect::FadeType::OUT));
        AddObject(fadeObj);
    }

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

    UpdateLoadedMaps();
    quitRequested = inputManager.QuitRequested();
}

void WorldState::Render() {
    bgObj->Render();

    auto currentTileMap = (TileMap *) (maps.size() > 0 ? maps[currentMapIndex].GetTileMap()->GetComponent(TILE_MAP_TYPE) : nullptr);
    auto mapDepth = currentTileMap == nullptr ? 0 : currentTileMap->GetDepth();
    auto layersDepth = objectLayers.rbegin() != objectLayers.rend() ? (*objectLayers.rbegin()).first + 1 : 0;
    for (int i = 0; i < (mapDepth > layersDepth ? mapDepth : layersDepth); i++) {
        auto it = objectLayers.find(i);
        
        if (maps.size() > 0) {
            auto tileMap = (TileMap *)maps[currentMapIndex].GetTileMap()->GetComponent(TILE_MAP_TYPE);
            if (i < tileMap->GetDepth()) {
                tileMap->RenderLayer(i);
            }
            auto prevIndex = currentMapIndex - 1;
            if (prevIndex >= 0) {
                tileMap = (TileMap *)maps[prevIndex].GetTileMap()->GetComponent(TILE_MAP_TYPE);
                if (i < tileMap->GetDepth()) {
                    tileMap->RenderLayer(i);
                }
            }
            auto nextIndex = currentMapIndex + 1;
            if (nextIndex < maps.size()) {
                tileMap = (TileMap *)maps[nextIndex].GetTileMap()->GetComponent(TILE_MAP_TYPE);
                if (i < tileMap->GetDepth()) {
                    tileMap->RenderLayer(i);
                }
            }
        }

        if (it != objectLayers.end()) {
            auto &objects = (*it).second;

            for (auto &object : objects) {
                object->Render();
            }
        }
    }
}

void WorldState::Start() {
    vector<string> m1 = { "map/1/ground.png", "map/1/rocks.png" };
    maps.emplace_back(m1, Map::MapDirection::DOWN, "map/1/collisionMap.txt");
//    maps.emplace_back("map/tileMap1.txt", "img/tileset1.png", Map::MapDirection::DOWN, "map/collisionMap.txt", "map/terrainMap.txt");

    StartArray();

    LoadMaps();
    bgMusic->Play();

}

void WorldState::Pause() {

}

void WorldState::Resume() {
    Camera::pos = Vec2(0, 0);
}

void WorldState::LoadAssets() {

}

weak_ptr<GameObject> WorldState::AddCollidable(shared_ptr<GameObject> object) {
    collidables.emplace_back(object);
    return weak_ptr<GameObject>(object);
}

weak_ptr<GameObject> WorldState::AddCollider(shared_ptr<GameObject> object) {
    colliders.emplace_back(object);
    return weak_ptr<GameObject>(object);
}

void WorldState::CheckCollisions() {
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

weak_ptr<GameObject> WorldState::AddObject(shared_ptr<GameObject> object) {
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

weak_ptr<GameObject> WorldState::AddObject(GameObject *object) {
    auto ptr = shared_ptr<GameObject>(object);

    return AddObject(ptr);
}

void WorldState::UpdateLoadedMaps() {
    auto player = Player::player;

    if (!maps[currentMapIndex].GetTileMap()->box.Contains(player->GetGameObject().box.Center())) {
        auto prevMap = currentMapIndex - 1;
        auto nextMap = currentMapIndex + 1;
        if (prevMap >= 0 && maps[prevMap].GetTileMap()->box.Contains(player->GetGameObject().box.Center())) {
            currentMapIndex = prevMap;
        } else if (nextMap < maps.size() && maps[nextMap].GetTileMap()->box.Contains(player->GetGameObject().box.Center())) {
            currentMapIndex = nextMap;
        } else {
            throw string("Player out of bounds");
        }
    }
}

void WorldState::LoadMaps() {
    for (int i = 0; i < maps.size()-1; ++i) {
        auto currentMap = maps[i];
        auto nextMap = maps[i+1];
        auto currentBox = currentMap.GetTileMap()->box;

        switch (nextMap.GetDirection()) {
            case Map::MapDirection::DOWN:
                nextMap.GetTileMap()->box.x = currentBox.x;
                nextMap.GetTileMap()->box.y = currentBox.y + currentBox.h;
                break;
            case Map::MapDirection::UP:
                nextMap.GetTileMap()->box.x = currentBox.x;
                nextMap.GetTileMap()->box.y = currentBox.y - currentBox.h;
                break;
            case Map::MapDirection::LEFT:
                nextMap.GetTileMap()->box.x = currentBox.x - currentBox.w;
                nextMap.GetTileMap()->box.y = currentBox.y;
                break;
            default:
                nextMap.GetTileMap()->box.x = currentBox.x + currentBox.w;
                nextMap.GetTileMap()->box.y = currentBox.y;
        }
    }
}

Map &WorldState::GetCurrentMap() {
    return maps[currentMapIndex];
}
