//
// Created by fabio on 20/05/18.
//

#ifndef NEPHESH_PLAYER_H
#define NEPHESH_PLAYER_H


#include "GameObject.h"
#include "Vec2.h"
#include "Timer.h"

#define PLAYER_TYPE "Player"

using namespace std;

#define PLAYER_MAGIC_SPRITE_COUNT 6
#define PLAYER_WALK_SPRITE_COUNT 6
#define PLAYER_IDLE_SPRITE_COUNT 6
#define PLAYER_ATTACK_SPRITE_COUNT 6
#define PLAYER_ATTACK_DURATION 0.3
#define PLAYER_ATTACK_RANGE 35
#define PLAYER_ATTACK_WIDTH 60
#define PLAYER_SPEED 100
#define PLAYER_SPR_TIME 0.1

class Player : public Component {
private:
    enum PlayerState {
        ATTACKING,
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
    vector<pair<PlayerDirection, string>> attackAnimations;

    PlayerDirection GetNewDirection(vector<PlayerDirection> directions);
    PlayerDirection GetDirection(Vec2 target);
    void Shoot();
    void Attack();
    PlayerState state;
    Vec2 speed;
    int hp;

    Timer timer;

    string GetMovementAnimation();
    string GetShootingAnimation();
    string GetAttackAnimation();
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
