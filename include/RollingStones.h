//
// Created by fabio on 06/07/18.
//

#ifndef NEPHESH_ROLLINGROCKS_H
#define NEPHESH_ROLLINGROCKS_H

#define ROLLINGROCKS_SPRITE ""

#include "Component.h"
#include "Player.h"
#include <cmath>

using namespace std;

class RollingStones : public Component {
public:

    RollingStones(GameObject &associated, int damage, float speed, float maxDist = 0);

    ~RollingStones() override;

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void Start() override;

    void NotifyCollision(GameObject &other) override;

private:
    float targetAngleDeg;
    float speed;
    Vec2 speedVec;
    Timer timer;
    int damage;
    float maxDist;
    float distCovered;
};


#endif //NEPHESH_ROLLINGROCKS_H
