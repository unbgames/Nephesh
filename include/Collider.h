//
// Created by ftfnunes on 17/04/18.
//

#ifndef T1_COLLIDER_H
#define T1_COLLIDER_H

#include <functional>
#include "GameObject.h"

#define COLLIDER_TYPE "Collider"
#define DEBUG

class Collider : public Component {
public:
    explicit Collider(GameObject &associated, Vec2 scale = Vec2(1, 1), Vec2 offset = Vec2(0, 0));

    Rect box;

    void Update(float dt) override;
    void Render() override;
    bool Is(string type) override;

    void SetScale(Vec2 scale);
    void SetOffset(Vec2 offset);

    void SetCanCollide(function<bool(GameObject& collidable)> canCollide);
    bool CanCollide(GameObject& collidable);

private:
    Vec2 scale;
    Vec2 offset;

    // default: return true
    function<bool(GameObject& collidable)> canCollide;
};


#endif //T1_COLLIDER_H
