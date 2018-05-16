//
// Created by ftfnunes on 09/05/18.
//

#ifndef T1_EVENTTIMER_H
#define T1_EVENTTIMER_H

using namespace std;

#include <functional>
#include "Component.h"
#include "Timer.h"

#define EVENT_TIMER_TYPE "EventTimer"

class IntervalTimer : public Component {
public:
    IntervalTimer(GameObject &associated, float timeLimit, function<void()> callback);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

private:
    Timer timer;
    float timeLimit;
    function<void()> callback;
};


#endif //T1_EVENTTIMER_H
