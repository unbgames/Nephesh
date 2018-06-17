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
#define WALK_SPRITE_DURATION 0.6
#define IDLE_SPRITE_COUNT 6
#define IDLE_SPRITE_DURATION 1
#define ATTACK_SPRITE_COUNT 6
#define ATTACK_DURATION 0.3
#define ATTACK_RANGE 70
#define ATTACK_WIDTH 120
#define CHARGING_DURATION 0.3
#define CHARGING_SPRITE_COUNT 0.3
#define IDLE_SPRITE "img/idle_up.png"

class Player : public Component {
private:
    enum PlayerState {
        STARTING,
        TALKING,
        ATTACKING,
        CHARGING,
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

    class PlayerStateData {
    public:
        PlayerDirection direction;
        string animation;
        Vec2 playerSpriteScale;
        Vec2 playerSpriteOffset;
        Vec2 objectSpriteOffset;

        PlayerStateData (PlayerDirection direction, string movementAnimation, Vec2 scale, Vec2 offset, Vec2 objectSpriteOffset = Vec2());
    };

    PlayerDirection currentDirection;

    vector<PlayerStateData> movingData;
    vector<PlayerStateData> attackingData;
    vector<PlayerStateData> shootingData;
    vector<PlayerStateData> chargingData;
    vector<PlayerStateData> idleData;

    PlayerDirection GetNewDirection(vector<PlayerDirection> directions);
    PlayerDirection GetNewDirection(Vec2 target);

    PlayerStateData ChangeDirection();

    void Shoot();
    void Attack();
    void Charge();

    Vec2 target;
    PlayerState state;
    float closestNpcDistance;
    bool shouldStopTalking;
    Vec2 speed;
    int hp;
    Vec2 lastPos;

    Timer timer;

    PlayerStateData GetStateData(vector<PlayerStateData> data);
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
