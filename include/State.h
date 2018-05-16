#include <memory>
#include <vector>
#include <map>

#ifndef T1_STATE_H
#define T1_STATE_H


#include "GameObject.h"

class State {
public:
    State();
    virtual ~State();

    virtual void LoadAssets() = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

    virtual void Start() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;

    weak_ptr<GameObject> AddObject(GameObject *object);
    weak_ptr<GameObject> AddObject(shared_ptr<GameObject> object);
    weak_ptr<GameObject> GetObjectPtr(GameObject *object);
    shared_ptr<GameObject> PopObjectPtr(GameObject *obj);

    bool PopRequested();
    bool QuitRequested();

protected:
    bool popRequested;
    bool quitRequested;
    bool started;

    virtual void StartArray();
    virtual void UpdateArray(float dt);
    virtual void RenderArray();

    vector<weak_ptr<GameObject>> addedObjects;
    map<int, vector<shared_ptr<GameObject>>> objectLayers;
};

#endif //T1_STATE_H
