//
// Created by ftfnunes on 04/07/18.
//

#ifndef NEPHESH_FADEINEFFECT_H
#define NEPHESH_FADEINEFFECT_H


#include "GameObject.h"
#include "Timer.h"

#define FADE_IN_EFFECT_TYPE "FadeInEffect"

class FadeEffect : public Component {
public:
    enum FadeType {
        IN,
        OUT
    };
    FadeEffect(GameObject &associated, float duration, float offset, function<void()> callback = [] {}, FadeType type = IN);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;
private:
    float duration;
    float timeElapsed;
    float offset;
    Timer timer;
    function<void()> callback;
    FadeType type;
    bool finished;
};


#endif //NEPHESH_FADEINEFFECT_H
