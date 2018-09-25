//
// Created by ftfnunes on 19/05/18.
//

#ifndef NEPHESH_RAYPROJECTILE_H
#define NEPHESH_RAYPROJECTILE_H

#include <GameObject.h>
#include "Player.h"

#define BEAM_SKILL_TYPE "BeamSkill"
#define BEAM_LIFETIME 0.3
#define CHARGING_DURATION 0.3

class BeamSkill : public Component {
public:

    BeamSkill(GameObject &associated, Vec2 target, Player::PlayerDirection direction);

    ~BeamSkill() override;

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void Start() override;

    void Fire();

    void NotifyCollision(GameObject &other) override;

private:
    weak_ptr<GameObject> initObject;
    weak_ptr<GameObject> endObject;
    Player::PlayerDirection direction;

    Vec2 target;
    Timer timer;
    float cutoffPoint;
    bool lockBeam;
};


#endif //NEPHESH_RAYPROJECTILE_H
