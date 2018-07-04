//
// Created by fabio on 20/05/18.
//

#ifndef NEPHESH_PLAYER_H
#define NEPHESH_PLAYER_H

#include "GameObject.h"
#include "Vec2.h"
#include "Timer.h"

using namespace std;


#define DASH_DURATION 0.35
#define DASH_SPRITE_COUNT 4
#define DASH_SPEED 1000
#define MAGIC_SPRITE_COUNT 6
#define MAGIC_SPRITE_DURATION 0.3
#define WALK_SPRITE_COUNT 6
#define WALK_SPRITE_DURATION 0.6
#define IDLE_SPRITE_COUNT 6
#define IDLE_SPRITE_DURATION 1.2
#define ATTACK_SPRITE_COUNT 6
#define ATTACK_DURATION 0.3
#define STEP_INTERVAL 0.33
#define ATTACK_RANGE 70
#define ATTACK_WIDTH 120
#define IDLE_SPRITE "img/idle_up.png"

class Player : public Component {
public:

    // Indicates in which direction the player is facing
    enum PlayerDirection {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    void Start() override;

    weak_ptr<GameObject> closestNpc;

    explicit Player(GameObject& associated);
    ~Player() override;
    
    void Update(float dt) override;
    void Render() override;
    bool Is(string type) override;

    void StopTalking();
    
    static Player* player;

    void NotifyCollision(GameObject& other) override;
private:
    enum PlayerState {
        //Starting state of player
        STARTING,
        // Stays in this state while player is dashing
        DASHING,
        //Stays in this state while an NPC is talking
        TALKING,
        //Stays in this state while the melee attack is being performed (as long as the attack animation lasts)
        ATTACKING,
        //Stays in this state while the magic attack is being performed (as long as the attack animation lasts)
        SHOOTING,
        //Stays in this state while a movement key is pressed (W, A, S, D)
        MOVING,
        //When the user is not doing anything
        IDLE,
    };

    //This internal class stores information about how to render the player in each state.
    class PlayerStateData {
    public:
        //This identifies the direction that this object must be used
        PlayerDirection direction;
        //The animation that must be shown when the player faces this direction
        string animation;
        //The scale that must be applyied to the player collider and collidable
        Vec2 playerSpriteScale;
        //The offset that must be applyied to the player collider and collidable
        Vec2 playerSpriteOffset;
        //The offset that must be applyied to any object that is associated with this state
        Vec2 objectSpriteOffset;

        PlayerStateData (PlayerDirection direction, string movementAnimation, Vec2 scale, Vec2 offset, Vec2 objectSpriteOffset = Vec2());
    };

    PlayerDirection currentDirection;

    //Collection of state information relative to the MOVING state
    vector<PlayerStateData> movingData;
    //Collection of state information relative to the ATTACKING state, objectSpriteOffset references the melee attack object that is spawned by this state
    vector<PlayerStateData> attackingData;
    //Collection of state information relative to the SHOTTING state, objectSpriteOffset references the beamSkill object that is spawned by this state
    vector<PlayerStateData> shootingData;
    //Collection of state information relative to the IDLE state
    vector<PlayerStateData> idleData;
    //Collection of state information relative to the DASHING state
    vector<PlayerStateData> dashingData;

    vector<string> grassStepSounds;
    vector<string> stoneStepSounds;
    vector<string> dirtStepSounds;
    vector<string> dashSounds;
    vector<string> attackMissSounds;
    vector<string> attackHitSounds;

    //Get a direction for the player based on the pressed directions in this tick
    PlayerDirection GetNewDirection(vector<PlayerDirection> directions);
    //Get a direction for the player based on a position in this tick
    PlayerDirection GetNewDirection(Vec2 target);

    //Changes the player sprite configuration based on the current direction and state
    PlayerStateData ChangeDirection();

    void Shoot();
    void Attack();
    void Dash();

    PlayerState state;

    //Used to calculate which NPC should talk in the next tick
    float closestNpcDistance;

    //An NPC sets this bool to true when it is done talking
    bool shouldStopTalking;

    //Indicates if the player is in the middle of the magic animation
    bool preparing;
    bool attacked;

    Vec2 speed;
    int hp;

    Vec2 target;

    Timer timer;

    weak_ptr<GameObject> meleeAttack;

    PlayerStateData GetStateData(vector<PlayerStateData> data);
    void SetSprite(string file, int frameCount, float frameTime, bool flip = false);
    void PlaySound(string file);
    string GetRandomStepSound();
};


#endif //NEPHESH_PLAYER_H
