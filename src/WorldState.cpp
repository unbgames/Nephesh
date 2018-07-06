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
#include <Boss.h>
#include "WorldState.h"
#include "Text.h"

WorldState::WorldState() : State(), currentMapIndex(0) {
    auto obj = new GameObject();
    bgObj = shared_ptr<GameObject>(obj);
    bgObj->AddComponent(new Sprite(*bgObj, "img/whiteBg.png"));
    bgObj->AddComponent(new CameraFollower(*bgObj));

    auto playerObj = new GameObject();
    playerObj->box += WORLD_PLAYER_INITIAL_POSITION;
    playerObj->AddComponent(new Player(*playerObj));
    AddObject(playerObj);

    Camera::Follow(playerObj);

    bgMusic = new Music("audio/mundo.ogg");
    Mix_VolumeMusic(32);

    Player::player->Freeze();
    auto fadeInObj = new GameObject(2);
    fadeInObj->AddComponent(new FadeEffect(*fadeInObj, WORLD_FADE_IN_DURATION, 2, [] { Player::player->Unfreeze(); }));
    AddObject(fadeInObj);

    auto bossObj = new GameObject();
    bossObj->box.x = WIDTH/2;      
    bossObj->box.y = HEIGHT/4;
    bossObj->AddComponent(new Boss(*bossObj));
    bossObj->AddComponent(new Collidable(*bossObj));
    AddObject(bossObj);
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

        fadeObj->AddComponent(new FadeEffect(*fadeObj, WORLD_FADE_OUT_DURATION, 0, callback, FadeEffect::FadeType::OUT));
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
    vector<string> m2 = { "map/2/ground.png", "map/2/rocks.png" };
    vector<string> m3 = { "map/3/ground.png", "map/3/rocks.png" };
    vector<string> m4 = { "map/4/ground.png", "map/4/rocks.png" };
    vector<string> m5 = { "map/5/ground.png", "map/5/rocks.png" };
    vector<string> m8 = { "map/8/ground.png", "map/8/rocks.png" };
    maps.emplace_back(m1, Map::MapDirection::DOWN, "map/1/collisionMap.txt", "map/1/terrainMap.txt");
    maps.emplace_back(m2, Map::MapDirection::DOWN, "map/2/collisionMap.txt", "map/2/terrainMap.txt");
    maps.emplace_back(m3, Map::MapDirection::RIGHT, "map/3/collisionMap.txt", "map/3/terrainMap.txt");
    maps.emplace_back(m4, Map::MapDirection::UP, "map/4/collisionMap.txt", "map/4/terrainMap.txt");
    maps.emplace_back(m5, Map::MapDirection::RIGHT, "map/5/collisionMap.txt", "map/5/terrainMap.txt");
    maps.emplace_back(m8, Map::MapDirection::RIGHT, "map/8/collisionMap.txt", "map/8/terrainMap.txt");

    StartArray();

    LoadMaps();
    LoadNpcs();
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

        if (nextMap.GetTileMap()->box.Contains(Player::player->GetGameObject().box.Center())) {
            currentMapIndex = i;
        }
    }
}

Map &WorldState::GetCurrentMap() {
    return maps[currentMapIndex];
}

void WorldState::LoadNpcs() {
    auto creatureObj1 = new GameObject();
    creatureObj1->AddComponent(new Sprite(*creatureObj1, "img/criatura.png", 6, 0.2));
    creatureObj1->AddComponent(new Npc(*creatureObj1, "npcs/criatura_magica1.txt"));
    creatureObj1->AddComponent(new Sound(*creatureObj1, "audio/npcs/criatura_magica_1.wav"));
    AddObject(creatureObj1);

    auto creatureObj2 = new GameObject();
    creatureObj2->AddComponent(new Sprite(*creatureObj2, "img/criatura.png", 6, 0.2));
    creatureObj2->AddComponent(new Npc(*creatureObj2, "npcs/criatura_magica2.txt", true));
    creatureObj2->AddComponent(new Sound(*creatureObj2, "audio/npcs/criatura_magica_1.wav"));
    AddObject(creatureObj2);
}