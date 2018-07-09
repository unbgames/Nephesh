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
#include "Debug.h"

WorldState::WorldState() : State(), currentMapIndex(0) {
    auto obj = new GameObject();
    bgObj = shared_ptr<GameObject>(obj);
    bgObj->AddComponent(new Sprite(*bgObj, "img/bg.png"));
    bgObj->AddComponent(new CameraFollower(*bgObj));

    auto playerObj = new GameObject();
    playerObj->box += WORLD_PLAYER_INITIAL_POSITION;
    playerObj->AddComponent(new Player(*playerObj));
    playerObj->AddComponent(new Collidable(*playerObj, {0.4, 1}));
    AddObject(playerObj);

    Camera::Follow(playerObj);
    Camera::SetCameraHeight(1);
    Camera::SetLayerDepth(4, 0.92);

    bgMusic = new DecentMusic("audio/mundo.ogg");
    Mix_VolumeMusic(32);

    Player::player->Freeze();
    auto fadeInObj = new GameObject(WORLD_LAST_LAYER);
    fadeInObj->AddComponent(new FadeEffect(*fadeInObj, WORLD_FADE_IN_DURATION, 2, [] { Player::player->Unfreeze(); }));
    AddObject(fadeInObj);

    bossObj = new GameObject(1);
    bossObj->box += WORLD_BOSS_INITIAL_POSITION;
    bossObj->AddComponent(new Boss(*bossObj));
    bossObj->AddComponent(new Collidable(*bossObj));
//  bossObj->AddComponent(new Debug(*bossObj));
    AddObject(bossObj);
}

WorldState::~WorldState() = default;

void WorldState::Update(float dt) {
    bgObj->Update(dt);
    Camera::Update(dt);
    auto& inputManager = InputManager::GetInstance();
    
    if(ending){
        endingTimer.Update(dt);

        if(endingTimer.Get() >= endingMusicLength-0.5){
            bgMusic->Stop(0);
            bgMusic->Open("audio/logo_sound.wav");
            bgMusic->Play(1, 0);

            ending = false;
            endingTimer.Restart();
        }
    }
    
    if(bossObj != nullptr){
        auto compBoss = (Boss*)bossObj->GetComponent(BOSS_TYPE);
        if(compBoss != nullptr && compBoss->GetHp() <= 0){
            bgMusic->Stop(0);
            bgMusic->Open("audio/black_out.wav");
            bgMusic->Play(1, 0);
            endingTimer.Restart();
            ending = true;
            bossObj = nullptr;
            Player::player->Freeze();
        }
    }
    
    if(bossMusicToPlay){
        introMusicTimer.Update(dt);
        
        if(introMusicTimer.Get() >= introMusicLength-0.5){
            bgMusic->Stop(0);
            bgMusic->Open("audio/first_encounter_loop.wav");
            bgMusic->Play(0, 0);

            bossMusicToPlay = false;
            introMusicTimer.Restart();
            
        }
    }

    if (inputManager.KeyPress(SDLK_t)) {
        auto mousePos = inputManager.GetMouse();

        auto blockObj = new GameObject();
        blockObj->angleDeg = 45;
        blockObj->AddComponent(new Sprite(*blockObj, "img/block.png"));
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
    vector<string> m1 = { "map/1/ground.png", "map/1/rocks.png", "map/1/trees.png", "map/1/surroundings.png", "map/1/lighting.png", "map/1/view.png" };
    vector<string> m2 = { "map/2/ground.png", "map/2/rocks.png", "map/2/trees.png", "map/2/surroundings.png", "map/2/lighting.png", "map/2/view.png" };
    vector<string> m3 = { "map/3/ground.png", "map/3/rocks.png", "map/3/trees.png", "map/3/surroundings.png", "map/3/lighting.png", "map/3/view.png", "map/3/vines.png" };
    vector<string> m4 = { "map/4/ground.png", "map/4/rocks.png", "map/4/trees.png", "map/4/surroundings.png", "map/4/lighting.png", "map/4/view.png", "map/4/vines.png" };
    vector<string> m5 = { "map/5/ground.png", "map/5/rocks.png", "map/5/trees.png", "map/5/surroundings.png", "map/5/lighting.png", "map/5/view.png", "map/5/vines.png" };
    vector<string> m8 = { "map/8/ground.png", "map/8/rocks.png", "map/8/trees.png", "map/8/surroundings.png", "map/8/lighting.png", "map/8/vines.png" };
    vector<string> m9 = { "map/9/ground.png", "map/9/rocks.png", "map/9/trees.png", "map/9/surroundings.png", "map/9/lighting.png", "map/9/view.png", "map/9/vine.png" };
    maps.emplace_back(m1, Map::MapDirection::DOWN, "map/1/collisionMap.txt", "map/1/terrainMap.txt");
    maps.emplace_back(m2, Map::MapDirection::DOWN, "map/2/collisionMap.txt", "map/2/terrainMap.txt");
    maps.emplace_back(m3, Map::MapDirection::RIGHT, "map/3/collisionMap.txt", "map/3/terrainMap.txt");
    maps.emplace_back(m4, Map::MapDirection::UP, "map/4/collisionMap.txt", "map/4/terrainMap.txt");
    maps.emplace_back(m5, Map::MapDirection::RIGHT, "map/5/collisionMap.txt", "map/5/terrainMap.txt");
    maps.emplace_back(m8, Map::MapDirection::RIGHT, "map/8/collisionMap.txt", "map/8/terrainMap.txt");
    maps.emplace_back(m9, Map::MapDirection::UP, "map/9/collisionMap.txt", "map/9/terrainMap.txt");

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
    Resources::GetSound("audio/first_encounter_loop.wav");
    Resources::GetSound("audio/first_encounter_intro.wav");
    Resources::GetSound("audio/mundo.ogg");
    Resources::GetSound("audio/mundo.ogg");
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

                if( (collidable) && (collider->GetLayer() == collidable->GetLayer() || collidable->HasComponent(COLLISION_MAP_TYPE)) && (colliderCpt->CanCollide(*collidable)) ){
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
    
    UpdateMusic(prevIndex);
}

void WorldState::UpdateMusic(int prevIndex) {
    if(prevIndex != currentMapIndex && currentMapIndex == bossMapIndex){
        bgMusic->Stop(500);
        //musicChangeTimer.Restart();
        bgMusic->Open("audio/first_encounter_intro.wav");
        bgMusic->Play(1, 1000);
        
        introMusicTimer.Restart();
        bossMusicToPlay = true;
        cout << "Musica boss." << endl;
    } else if(prevIndex != currentMapIndex && prevIndex == bossMapIndex){
        bgMusic->Stop(500);
        bossMusicToPlay = false;
        //musicChangeTimer.Restart();
        bgMusic->Open("audio/mundo.ogg");
        bgMusic->Play(0, 2000);
        cout << "Musica mundo." << endl;
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
    vector<string> creatureSounds1 {
            "audio/npcs/criatura/criatura_magica_4.wav",
            "audio/npcs/criatura/criatura_magica_3.wav"
    };

    auto creatureObj1 = new GameObject();
    creatureObj1->AddComponent(new Sprite(*creatureObj1, "img/criatura.png", 6, 0.2));
    creatureObj1->AddComponent(new Npc(*creatureObj1, "npcs/criatura_magica1.txt", creatureSounds1));
    AddObject(creatureObj1);

    vector<string> creatureSounds2 {
            "audio/npcs/criatura/criatura_magica_4.wav",
            "audio/npcs/criatura/criatura_magica_2.wav",
            "audio/npcs/criatura/criatura_magica_1.wav"
    };

    auto creatureObj2 = new GameObject();
    creatureObj2->AddComponent(new Sprite(*creatureObj2, "img/criatura.png", 6, 0.2));
    creatureObj2->AddComponent(new Npc(*creatureObj2, "npcs/criatura_magica2.txt", creatureSounds2));
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
