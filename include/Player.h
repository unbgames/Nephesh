//
// Created by fabio on 20/05/18.
//

#ifndef NEPHESH_PLAYER_H
#define NEPHESH_PLAYER_H


#include "GameObject.h"
#include "Vec2.h"

using namespace std;

#define MAGIC_SPRITE_COUNT 6
#define WALK_SPRITE_COUNT 6
#define IDLE_SPRITE_COUNT 6
#define ATTACK_SPRITE_COUNT 6
#define ATTACK_DURATION 0.5
#define ATTACK_RANGE 30
#define ATTACK_WIDTH 60

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

};


#endif //NEPHESH_PLAYER_H
