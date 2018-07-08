#ifndef T1_TITLESTATE_H
#define T1_TITLESTATE_H


#include "State.h"
#include "TileMap.h"
#include "Map.h"
#include "Music.h"
#include <vector>
#include <memory>

using namespace std;

#define WORLD_FADE_IN_DURATION 1
#define WORLD_FADE_OUT_DURATION 1
#define WORLD_MUSIC_FADE_OUT_TIME 1500
#define WORLD_PLAYER_INITIAL_POSITION Vec2(1024, 1024)

class WorldState : public State {
    public:

        WorldState();
        ~WorldState();

        void LoadAssets();

        void Update(float dt) override;

        void Render() override;

        void Start() override;

        void Pause() override;

        void Resume() override;

        void CheckCollisions();

        weak_ptr<GameObject> AddObject(shared_ptr<GameObject> object) override;
        weak_ptr<GameObject> AddObject(GameObject *object) override;

        weak_ptr<GameObject> AddCollidable(shared_ptr<GameObject> object);

        weak_ptr<GameObject> AddCollider(shared_ptr<GameObject> object);

        Map &GetCurrentMap();
        int GetCurrentMapIndex();

private:
        vector<weak_ptr<GameObject>> collidables;
        vector<weak_ptr<GameObject>> colliders;

        shared_ptr<GameObject> bgObj;

        int currentMapIndex;
        vector<Map> maps;

        Music *bgMusic;

        void UpdateLoadedMaps();
        void LoadMaps();
        void LoadNpcs();
        void UpdateMusic(int prevIndex, int currentMapIndex);
        int bossMapIndex = 1;
        
        string musicToPlay = "";
        int fadeIn = -1;
        Timer musicChangeTimer;
};


#endif //T1_TITLESTATE_H
