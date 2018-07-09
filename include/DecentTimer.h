//
// Created by bruno on 08/07/18.
//

#ifndef NEPHESH_DECENTTIMER_H
#define NEPHESH_DECENTTIMER_H


#include <functional>
#include "GameObject.h"
#include "Timer.h"

#define DECENT_TIMER_TYPE "DecentTimer"

class DecentTimer: public Component {
public:
    DecentTimer(GameObject &associated, float timeLimit, function<void()> callback);

    void Update(float dt) override;
    void Render() override;
    bool Is(string type) override;

    static void ProgramTimer (GameObject &associated, float timeLimit, function<void()> callback);

private:
    Timer timer;
    float timeLimit;
    function<void()> callback;

};


#endif //NEPHESH_DECENTTIMER_H
