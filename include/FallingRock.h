//
// Created by bruno on 08/07/18.
//

#ifndef NEPHESH_FALLINGROCK_H
#define NEPHESH_FALLINGROCK_H

#include "CameraShaker.h"

#define FALLING_ROCK_TYPE "FallingRock"
#define FALLING_ROCK_MAX_RADIUS (GAME_HEIGHT*0.4)
#define FALLING_ROCK_ACCELERATION 2000
#define FALLING_ROCK_SPRITE_1 "img/big_rock_1.png"
#define FALLING_ROCK_SPRITE_2 "img/big_rock_2.png"
#define FALLING_ROCK_SPRITE_3 "img/big_rock_3.png"
#define FALLING_ROCK_SHADOW_SPRITE "img/shadow.png"
#define FALLING_ROCK_BREAKING_SPRITE "img/big_rock_breaking.png"
#define FALLING_ROCK_DUST_SPRITE "img/dust.png"

#define FALLING_ROCK_BREAKING_SOUND_1 "audio/boss/falling_rock_1.wav"
#define FALLING_ROCK_BREAKING_SOUND_2 "audio/boss/falling_rock_2.wav"

class FallingRock : public Component {
public:
    explicit FallingRock(GameObject& associated, int damage);

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    bool Is(string type) override;
    void NotifyCollision(GameObject &other) override;

private:
    Vec2 targetPos;
    GameObject *rockShadow;
    float linearSpeed;
    CameraShaker* camShaker;
    
    int damage;
};


#endif //NEPHESH_FALLINGROCK_H
