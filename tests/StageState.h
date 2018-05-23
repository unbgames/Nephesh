#include <memory>
#include <vector>
#include <map>
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

private:
    Music backgroundMusic;
    TileMap *tileMap;
    GameObject *bg;
    vector<GameObject> collidables;
    vector<GameObject> colliders;
};

#endif