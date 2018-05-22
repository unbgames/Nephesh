//
// Created by ftfnunes on 16/04/18.
//

#ifndef T1_PENGUINBODY_H
#define T1_PENGUINBODY_H

#include <memory>
#include "GameObject.h"
#include "Vec2.h"

#define ACCELERATION 200
#define MAP_WIDTH 1408
#define MAP_HEIGHT 1280
#define MAX_SPEED 500
#define BODY_ANGULAR_SPEED 150
#define BODY_TYPE "PenguinBody"

class PenguinBody : public Component {
public:
    explicit PenguinBody(GameObject &associated);

    ~PenguinBody() override;

    void NotifyCollision(GameObject &other) override;

    void Start() override;

    void Update(float dt) override;

    void Render() override;

    bool Is(string type) override;

    void Damage(int damage);

    Vec2 GetPosition();

    static PenguinBody *player;

private:
    weak_ptr<GameObject> pCannon;
    Vec2 speed;
    float linearSpeed;
    float angle;
    int hp;
};


#endif //T1_PENGUINBODY_H
