//
// Created by fabio on 20/05/18.
//

#ifndef NEPHESH_PLAYER_H
#define NEPHESH_PLAYER_H


#include "GameObject.h"
#include "Vec2.h"

using namespace std;

class Player : public Component {
    enum PlayerState{
        MOVING_UP,
        MOVING_DOWN,
        MOVING_LEFT,
        MOVING_RIGHT,
        IDLE
    };
    PlayerState state;
    Vec2 speed;
    //float linearSpeed;
    int hp;
    
    bool lessMouseX;
    bool lessMouseY;
    
    void SetSprite(string file, int frameCount, float frameTime, bool flip = false);

public:
    void Start() override;

    Player(GameObject& associated);
    ~Player();       
    
    void Update(float dt) override;
    void Render() override;
    bool Is(string type) override;
    
    static Player* player;

    void NotifyCollision(GameObject& other) override;

};


#endif //NEPHESH_PLAYER_H
