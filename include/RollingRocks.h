//
// Created by fabio on 06/07/18.
//

#ifndef NEPHESH_ROLLINGROCKS_H
#define NEPHESH_ROLLINGROCKS_H

#define ROLLINGROCKS_SPRITE ""

#include "Component.h"
#include "Player.h"

using namespace std;

class RollingRocks : public Component {
public:

    RollingRocks(GameObject &associated, int damage);

    ~RollingRocks() override;

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void Start() override;

    void NotifyCollision(GameObject &other) override;

private:
    Vec2 target;
    float targetAngle;
    Timer timer;
    int damage;
};


#endif //NEPHESH_ROLLINGROCKS_H
