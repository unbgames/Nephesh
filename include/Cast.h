//
// Created by ftfnunes on 06/07/18.
//

#ifndef NEPHESH_CAST_H
#define NEPHESH_CAST_H


#include <GameObject.h>
#include <Timer.h>

#define CAST_TYPE "Cast"

class Cast : public Component {
public:
    explicit Cast(GameObject &associated, GameObject *next, float duration);

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

private:
    Timer castTimer;
    GameObject *next;
    float duration;
};


#endif //NEPHESH_CAST_H
