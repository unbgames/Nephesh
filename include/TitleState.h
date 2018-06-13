#ifndef T1_TITLESTATE_H
#define T1_TITLESTATE_H


#include "State.h"

class TitleState : public State {
    public:
        TitleState();
        ~TitleState();

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
};


#endif //T1_TITLESTATE_H
