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
#include <Resources.h>
#include <CollisionMap.h>
#include "WorldState.h"
#include "Text.h"
#include "config.h"

WorldState::WorldState() : State(), currentMapIndex(0) {
    auto obj = new GameObject();
    bgObj = shared_ptr<GameObject>(obj);
    bgObj->AddComponent(new Sprite(*bgObj, ASSETS_PATH("/img/whiteBg.png") ));
    bgObj->AddComponent(new CameraFollower(*bgObj));

    auto playerObj = new GameObject();
    playerObj->box += WORLD_PLAYER_INITIAL_POSITION;
    playerObj->AddComponent(new Player(*playerObj));
    AddObject(playerObj);

    Camera::Follow(playerObj);
    Camera::SetCameraHeight(1);
    Camera::SetLayerDepth(4, 0.92);

    bgMusic = new Music( ASSETS_PATH("/audio/mundo.ogg") );
    Mix_VolumeMusic(32);

    Player::player->Freeze();
    auto fadeInObj = new GameObject(WORLD_LAST_LAYER);
    fadeInObj->AddComponent(new FadeEffect(*fadeInObj, WORLD_FADE_IN_DURATION, 2, [] { Player::player->Unfreeze(); }));
    AddObject(fadeInObj);

    auto bossObj = new GameObject();
    bossObj->box.x = 0.7*GAME_WIDTH;
    bossObj->box.y = GAME_HEIGHT/2;
    bossObj->AddComponent(new Boss(*bossObj));
    bossObj->AddComponent(new Collidable(*bossObj));
    AddObject(bossObj);
}

WorldState::~WorldState() = default;

void WorldState::Update(float dt) {
    bgObj->Update(dt);
    Camera::Update(dt);
    auto& inputManager = InputManager::GetInstance();
    
    musicChangeTimer.Update(dt);
    
    if(musicToPlay.size() > 0 && fadeIn >= 0 && musicChangeTimer.Get() > WORLD_MUSIC_FADE_OUT_TIME/1000.0){
        bgMusic->Open(musicToPlay);
        bgMusic->Play(-1, fadeIn);
        
        musicToPlay = "";
        fadeIn = -1;
        musicChangeTimer.Restart();
    }

    if (inputManager.KeyPress(SDLK_t)) {
        auto mousePos = inputManager.GetMouse();

        auto blockObj = new GameObject();
        blockObj->angleDeg = 45;
        blockObj->AddComponent(new Sprite(*blockObj, ASSETS_PATH("/img/block.png") ));
        blockObj->AddComponent(new Collidable(*blockObj));
        blockObj->SetCenter(Camera::pos +  mousePos);
        AddObject(blockObj);
    }
    
    if (inputManager.KeyPress(SDLK_f)) {
        Player::player->DecreaseHp(10);
    }
    if (inputManager.KeyPress(SDLK_i)) {
        Player::player->IncreaseHp(10);
    }

    if (inputManager.KeyPress(ESCAPE_KEY)) {
        auto fadeObj = new GameObject(WORLD_LAST_LAYER);
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
            auto collisionMap = (CollisionMap *)maps[currentMapIndex].GetTileMap()->GetComponent(COLLISION_MAP_TYPE);
            if (i < tileMap->GetDepth()) {
                tileMap->RenderLayer(i);
            }
            if (i < collisionMap->GetMapDepth()) {
                collisionMap->Render();
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
    vector<string> m1 = { ASSETS_PATH("/map/1/ground.png"), ASSETS_PATH("/map/1/rocks.png") };
    vector<string> m2 = { ASSETS_PATH("/map/2/ground.png"), ASSETS_PATH("/map/2/rocks.png") };
    vector<string> m3 = { ASSETS_PATH("/map/3/ground.png"), ASSETS_PATH("/map/3/rocks.png") };
    vector<string> m4 = { ASSETS_PATH("/map/4/ground.png"), ASSETS_PATH("/map/4/rocks.png") };
    vector<string> m5 = { ASSETS_PATH("/map/5/ground.png"), ASSETS_PATH("/map/5/rocks.png"), ASSETS_PATH("/map/5/trees.png"), ASSETS_PATH("/map/5/surroundings.png"), ASSETS_PATH("/map/5/lighting.png"), ASSETS_PATH("/map/5/view.png"), ASSETS_PATH("/map/5/vines.png") };
    vector<string> m8 = { ASSETS_PATH("/map/8/ground.png"), ASSETS_PATH("/map/8/rocks.png"), ASSETS_PATH("/map/8/trees.png"), ASSETS_PATH("/map/8/surroundings.png"), ASSETS_PATH("/map/8/lighting.png"), ASSETS_PATH("/map/8/vines.png") };
    vector<string> m9 = { ASSETS_PATH("/map/9/ground.png"), ASSETS_PATH("/map/9/rocks.png"), ASSETS_PATH("/map/9/trees.png"), ASSETS_PATH("/map/9/surroundings.png"), ASSETS_PATH("/map/9/lighting.png"), ASSETS_PATH("/map/9/view.png"), ASSETS_PATH("/map/9/vine.png") };
    maps.emplace_back(m1, Map::MapDirection::DOWN, ASSETS_PATH("/map/1/collisionMap.txt"), ASSETS_PATH("/map/1/terrainMap.txt"));
    maps.emplace_back(m2, Map::MapDirection::DOWN, ASSETS_PATH("/map/2/collisionMap.txt"), ASSETS_PATH("/map/2/terrainMap.txt"));
    maps.emplace_back(m3, Map::MapDirection::RIGHT, ASSETS_PATH("/map/3/collisionMap.txt"), ASSETS_PATH("/map/3/terrainMap.txt"));
    maps.emplace_back(m4, Map::MapDirection::UP, ASSETS_PATH("/map/4/collisionMap.txt"), ASSETS_PATH("/map/4/terrainMap.txt"));
    maps.emplace_back(m5, Map::MapDirection::RIGHT, ASSETS_PATH("/map/5/collisionMap.txt"), ASSETS_PATH("/map/5/terrainMap.txt"));
    maps.emplace_back(m8, Map::MapDirection::RIGHT, ASSETS_PATH("/map/8/collisionMap.txt"), ASSETS_PATH("/map/8/terrainMap.txt"));
    maps.emplace_back(m9, Map::MapDirection::UP, ASSETS_PATH("/map/9/collisionMap.txt"), ASSETS_PATH("/map/9/terrainMap.txt"));

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
    Resources::GetMusic( ASSETS_PATH("audio/first_encounter_loop.mp3") );
    Resources::GetMusic( ASSETS_PATH("audio/mundo.ogg") );
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
    auto prevIndex = currentMapIndex;
    
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
    
    //UpdateMusic(prevIndex, currentMapIndex);
}

void WorldState::UpdateMusic(int prevIndex, int currentMapIndex) {
    if(prevIndex != currentMapIndex && currentMapIndex == bossMapIndex){
        bgMusic->Stop(WORLD_MUSIC_FADE_OUT_TIME);
        musicChangeTimer.Restart();
        musicToPlay = ASSETS_PATH("/audio/first_encounter_loop.mp3");
        fadeIn = 2000;
    } else if(prevIndex != currentMapIndex && prevIndex == bossMapIndex){
        bgMusic->Stop(WORLD_MUSIC_FADE_OUT_TIME);
        musicChangeTimer.Restart();
        musicToPlay = ASSETS_PATH("/audio/mundo.ogg");
        fadeIn = 500;
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

int WorldState::GetCurrentMapIndex() {
    return currentMapIndex;
}

void WorldState::LoadNpcs() {
    vector<string> creatureSounds {
            ASSETS_PATH("/audio/npcs/criatura/criatura_magica_1.wav"),
            ASSETS_PATH("/audio/npcs/criatura/criatura_magica_2.wav"),
            ASSETS_PATH("/audio/npcs/criatura/criatura_magica_3.wav"),
            ASSETS_PATH("/audio/npcs/criatura/criatura_magica_4.wav"),
            ASSETS_PATH("/audio/npcs/criatura/criatura_magica_5.wav"),
    };

    auto creatureObj1 = new GameObject();
    creatureObj1->AddComponent(new Sprite(*creatureObj1, ASSETS_PATH("/img/criatura.png"), 6, 0.2));
    creatureObj1->AddComponent(new Npc(*creatureObj1, ASSETS_PATH("/npcs/criatura_magica1.txt"), creatureSounds));
    AddObject(creatureObj1);

    auto creatureObj2 = new GameObject();
    creatureObj2->AddComponent(new Sprite(*creatureObj2, ASSETS_PATH("/img/criatura.png"), 6, 0.2));
    creatureObj2->AddComponent(new Npc(*creatureObj2, ASSETS_PATH("/npcs/criatura_magica2.txt"), creatureSounds));
    AddObject(creatureObj2);
}

Map &WorldState::GetNextMap() {
    if (currentMapIndex + 1 < maps.size()) {
        return maps[currentMapIndex + 1];
    }
    return maps[currentMapIndex];
}

Map &WorldState::GetPreviousMap() {
    if (currentMapIndex - 1 >= 0) {
        return maps[currentMapIndex - 1];
    }

    return maps[currentMapIndex];
}
