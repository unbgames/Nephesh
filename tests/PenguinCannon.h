//
// Created by ftfnunes on 16/04/18.
//

#ifndef T1_PENGUINCANNON_H
#define T1_PENGUINCANNON_H


#include <memory>
#include "GameObject.h"
#include "Timer.h"

#define CANNON_TYPE "PenguinCannon"
#define PENGUIN_BULLET_SPEED 500
#define PENGUIN_BULLET_DISTANCE 1000
#define PENGUIN_BULLET_DAMAGE 10
#define SHOOTING_COOLDOWN 0.4

class PenguinCannon : public Component {
public:
    explicit PenguinCannon(GameObject &associated, weak_ptr<GameObject> penguinBody);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void Shoot();

private:
    weak_ptr<GameObject> pBody;
    float angle;
    Timer timer;
};


#endif //T1_PENGUINCANNON_H
