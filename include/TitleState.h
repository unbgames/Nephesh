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

};


#endif //T1_TITLESTATE_H
