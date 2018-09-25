//
// Created by bruno on 08/07/18.
//

#ifndef NEPHESH_FALLINGROCK_H
#define NEPHESH_FALLINGROCK_H

#include "Component.h"
#include "CameraShaker.h"

#define FALLING_ROCK_TYPE "FallingRock"
#define FALLING_ROCK_MAX_RADIUS (GAME_HEIGHT*0.4)
#define FALLING_ROCK_ACCELERATION 2000
#define FALLING_ROCK_SPRITE_1 "img/rock_1.png"
#define FALLING_ROCK_SPRITE_2 "img/rock_2.png"
#define FALLING_ROCK_SPRITE_3 "img/rock_3.png"
#define FALLING_ROCK_SHADOW_SPRITE "img/shadow.png"

class FallingRock : public Component {
public:
    explicit FallingRock(GameObject& associated);

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(string type) override;
    void NotifyCollision(GameObject &other) override;

private:
    Vec2 targetPos;
    GameObject *rockShadow;
    float linearSpeed;
    bool fell;
    CameraShaker* camShaker;

};


#endif //NEPHESH_FALLINGROCK_H
