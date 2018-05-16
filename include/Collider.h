//
// Created by ftfnunes on 17/04/18.
//

#ifndef T1_COLLIDER_H
#define T1_COLLIDER_H

#include "GameObject.h"

#define COLLIDER_TYPE "Collider"
#define DEBUG

class Collider : public Component {
public:
    Collider(GameObject &associated, Vec2 scale = Vec2(1, 1), Vec2 offset = Vec2(0, 0));

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    Rect box;

    void SetScale(Vec2 scale);

    void SetOffset(Vec2 offset);

private:
    Vec2 scale;
    Vec2 offset;
};


#endif //T1_COLLIDER_H
