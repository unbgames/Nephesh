//
// Created by bruno on 13/06/18.
//

#ifndef NEPHESH_BOSS_H
#define NEPHESH_BOSS_H

#include "Component.h"
#include "Vec2.h"
#include "Timer.h"

#define BOSS_TYPE "Boss"
#define BOSS_INITIAL_HP 30

class Boss : public Component {
public:
    explicit Boss(GameObject& associated);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

private:
    enum BossState {
        ATTACKING,
        MOVING,
        IDLE
    };

    int hp;
    Vec2 speed;
    Vec2 destination;
    BossState bossState;
    Timer timer;

};


#endif //NEPHESH_BOSS_H
