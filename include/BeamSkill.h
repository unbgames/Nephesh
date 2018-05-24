//
// Created by ftfnunes on 19/05/18.
//

#ifndef NEPHESH_RAYPROJECTILE_H
#define NEPHESH_RAYPROJECTILE_H

#include <GameObject.h>

#define BEAM_SKILL_TYPE "BeamSkill"
#define BEAM_LIFETIME 0.5

class BeamSkill : public Component {
public:
    BeamSkill(GameObject &associated, Vec2 target);

    ~BeamSkill() override;

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void Start() override;

    void NotifyCollision(GameObject &other) override;

private:
    weak_ptr<GameObject> initObject;
    weak_ptr<GameObject> endObject;
    Vec2 target;
    Timer lifeTimer;
    float cutoffPoint;
};


#endif //NEPHESH_RAYPROJECTILE_H
