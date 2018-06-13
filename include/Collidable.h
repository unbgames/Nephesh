//
// Created by bruno on 22/05/18.
//

#ifndef NEPHESH_COLLIDABLE_H
#define NEPHESH_COLLIDABLE_H

#include "GameObject.h"
#include "Component.h"
#include "Vec2.h"
#include "Rect.h"
#include "Collider.h"

#define COLLIDABLE_TYPE "Collidable"
//#define DEBUG

class Collidable : public Component {
public:
    explicit Collidable(GameObject &associated, Vec2 scale = Vec2(1, 1), Vec2 offset = Vec2(0, 0));

    Rect box;

    void Update(float dt) override;
    void Render() override;
    bool Is(string type) override;

    virtual bool IsColliding(Collider& collider);

    vector<pair<LineSegment, Vec2>> GetIntersections(Collider &collider);

    void SetScale(Vec2 scale);
    void SetOffset(Vec2 offset);

protected:
    Vec2 scale;
    Vec2 offset;

};


#endif //NEPHESH_COLLIDABLE_H
