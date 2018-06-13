//
// Created by fabio on 20/05/18.
//

#ifndef NEPHESH_PLAYER_H
#define NEPHESH_PLAYER_H


#include "GameObject.h"
#include "Vec2.h"

#define PLAYER_TYPE "Player"

using namespace std;

class Player : public Component {
private:
    enum PlayerState {
        SHOOTING,
        MOVING,
        IDLE
    };

    enum PlayerDirection {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    PlayerDirection currentDirection;
    vector<pair<PlayerDirection, string>> movementAnimations;
    vector<pair<PlayerDirection, string>> shootingAnimations;

    PlayerDirection GetNewDirection(vector<PlayerDirection> directions);
    void Shoot();
    PlayerState state;
    Vec2 speed;
    int hp;

    Timer shootingTimer;

    string GetMovementAnimation();
    string GetShootingAnimation();
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

    Vec2 GetCenter();

};


#endif //NEPHESH_PLAYER_H
