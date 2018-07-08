//
// Created by fabio on 06/07/18.
//

#ifndef NEPHESH_ROLLINGROCKS_H
#define NEPHESH_ROLLINGROCKS_H

#define RSTONE_SOLO_STONE_SPRITE "img/solo_rock.png"
#define RSTONE_SLIDING_UP_STONE_SPRITE "img/rock.png"

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
    
    void PlaySound();

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
