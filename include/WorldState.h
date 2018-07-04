#ifndef T1_TITLESTATE_H
#define T1_TITLESTATE_H


#include "State.h"
#include "TileMap.h"
#include "Map.h"
#include "Music.h"
#include <vector>
#include <memory>

using namespace std;

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

private:
        vector<weak_ptr<GameObject>> collidables;
        vector<weak_ptr<GameObject>> colliders;

        shared_ptr<GameObject> bgObj;

        int currentMapIndex;
        vector<Map> maps;

        Music *bgMusic;

        void UpdateLoadedMaps();
        void LoadMaps();
};


#endif //T1_TITLESTATE_H
