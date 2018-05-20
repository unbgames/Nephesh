//
// Created by ftfnunes on 19/05/18.
//

#ifndef NEPHESH_RAYPROJECTILE_H
#define NEPHESH_RAYPROJECTILE_H

#include <GameObject.h>

#define BEAM_SKILL_TYPE

class BeamSkill : public Component {
public:
    BeamSkill(GameObject &associated, Vec2 target);

    ~BeamSkill() override;

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void NotifyCollision(GameObject &other) override;

private:
    Vec2 target;
public:
    void Start() override;
};


#endif //NEPHESH_RAYPROJECTILE_H
