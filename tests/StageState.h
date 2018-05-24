#include <memory>
#include <vector>
#include <map>
#include <CollisionMap.h>
#include "Music.h"
#include "TileMap.h"
#include "State.h"

#ifndef STATE_CLASS
#define STATE_CLASS

class StageState : public State {
  public:
    StageState();
    ~StageState();

    void LoadAssets();

    void Update(float dt);

    void Render();

    void Start() override;

    void Pause() override;

    void Resume() override;

    void CheckCollisions();

    weak_ptr<GameObject> AddObject(GameObject *object) override;

    weak_ptr<GameObject> AddCollidable(shared_ptr<GameObject> object);

    weak_ptr<GameObject> AddCollider(shared_ptr<GameObject> object);

private:
    Music backgroundMusic;
    shared_ptr<GameObject> bg;
    shared_ptr<GameObject> map;
    vector<weak_ptr<GameObject>> collidables;
    vector<weak_ptr<GameObject>> colliders;
};

#endif