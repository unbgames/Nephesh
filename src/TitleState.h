//
// Created by ftfnunes on 30/06/18.
//

#ifndef NEPHESH_TITLESTATE_H
#define NEPHESH_TITLESTATE_H


#include <State.h>
#include <Music.h>

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

private:
    Music *bgMusic;
};


#endif //NEPHESH_TITLESTATE_H
