//
// Created by ftfnunes on 30/06/18.
//

#ifndef NEPHESH_TITLESTATE_H
#define NEPHESH_TITLESTATE_H


#include <State.h>
#include <Music.h>

#define TITLE_FADE_IN_DURATION 3
#define TITLE_START_FADE_OUT_DURATION 2
#define TITLE_QUIT_FADE_OUT_DURATION 1

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
    bool frozen;
};


#endif //NEPHESH_TITLESTATE_H
