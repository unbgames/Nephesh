//
// Created by fabio on 20/05/18.
//

#ifndef NEPHESH_PLAYER_H
#define NEPHESH_PLAYER_H

#include "GameObject.h"
#include "Vec2.h"
#include "Timer.h"

using namespace std;

#define MAGIC_SPRITE_COUNT 6
#define WALK_SPRITE_COUNT 6
#define IDLE_SPRITE_COUNT 6
#define ATTACK_SPRITE_COUNT 6
#define ATTACK_DURATION 0.3
#define ATTACK_RANGE 70
#define ATTACK_WIDTH 120
#define IDLE_SPRITE "img/idle_up.png"

class Player : public Component {
private:
    enum PlayerState {
        TALKING,
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
    vector<pair<PlayerDirection, Vec2>> directionScales;
    vector<pair<PlayerDirection, Vec2>> directionOffsets;

    PlayerDirection GetNewDirection(vector<PlayerDirection> directions);
    PlayerDirection GetDirection(Vec2 target);

    void Shoot();
    void Attack();
    PlayerState state;
    float closestNpcDistance;
    bool shouldStopTalking;
    Vec2 speed;
    int hp;
    Vec2 lastPos;

    Timer timer;

    Vec2 GetDirectionScale();
    Vec2 GetDirectionOffset();
    string GetMovementAnimation();
    string GetShootingAnimation();
    string GetAttackAnimation();
    void SetSprite(string file, int frameCount, float frameTime, bool flip = false);
public:
    void Start() override;

    weak_ptr<GameObject> closestNpc;

    Player(GameObject& associated);
    ~Player();       
    
    void Update(float dt) override;
    void Render() override;
    bool Is(string type) override;

    void StopTalking();
    bool IsTalking();
    
    static Player* player;

    void NotifyCollision(GameObject& other) override;

};


#endif //NEPHESH_PLAYER_H
