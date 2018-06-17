//
// Created by fabio on 20/05/18.
//

#include <Sprite.h>
#include <InputManager.h>
#include <BeamSkill.h>
#include <Game.h>
#include <Collider.h>
#include <MeleeAttack.h>
#include <Npc.h>
#include <Camera.h>
#include <CollisionMap.h>
#include "Player.h"

#define SPEED 200

Player *Player::player = nullptr;

Player::Player(GameObject &associated) : Component(associated), speed({0, 0}), state(STARTING), hp(100) {
    Sprite *spr = new Sprite(associated, IDLE_SPRITE, IDLE_SPRITE_COUNT, 0.1, 0, true);

    associated.AddComponent(spr);

    auto collider = new Collider(associated, Vec2(0.5, 0.92));
    collider->SetCanCollide([&] (GameObject &other) -> bool {
        return (other.HasComponent(NPC_TYPE) && InputManager::GetInstance().KeyPress(SPACE_KEY)) || other.HasComponent(COLLISION_MAP_TYPE);
    });

    associated.AddComponent(collider);
    associated.box.h = spr->GetHeight();
    associated.box.w = spr->GetWidth();

    associated.SetCenter({associated.box.x, associated.box.y});

    Player::player = this;

    movingData.emplace_back(RIGHT, "img/walk_side.png", Vec2(0.5, 0.92), Vec2(20, 0));
    movingData.emplace_back(LEFT, "img/walk_side.png", Vec2(0.5, 0.92), Vec2(-15, 0));
    movingData.emplace_back(UP, "img/walk_up.png", Vec2(0.4, 0.92), Vec2(0, 0));
    movingData.emplace_back(DOWN, "img/walk_down.png", Vec2(0.4, 0.92), Vec2(0, 0));

    shootingData.emplace_back(RIGHT, "img/magic_side.png", Vec2(0.5, 0.92), Vec2(20, 0), Vec2(65, -20));
    shootingData.emplace_back(LEFT, "img/magic_side.png", Vec2(0.5, 0.92), Vec2(-15, 0), Vec2(-60, -20));
    shootingData.emplace_back(UP, "img/magic_up.png", Vec2(0.4, 0.92), Vec2(0, 0), Vec2(0, -45));
    shootingData.emplace_back(DOWN, "img/magic_down.png", Vec2(0.4, 0.92), Vec2(0, 0), Vec2(5, 35));

    attackingData.emplace_back(RIGHT, "img/attack_side.png", Vec2(0.3, 0.92), Vec2(-10, 0), Vec2(60, 0));
    attackingData.emplace_back(LEFT, "img/attack_side.png", Vec2(0.3, 0.92), Vec2(10, 0), Vec2(-60, 0));
    attackingData.emplace_back(UP, "img/attack_up.png", Vec2(0.4, 0.92), Vec2(0, 0), Vec2(0, -40));
    attackingData.emplace_back(DOWN, "img/attack_down.png", Vec2(0.4, 0.92), Vec2(0, 0), Vec2(0, 30));

    idleData.emplace_back(RIGHT, "img/idle_down.png", Vec2(0.5, 0.92), Vec2(25, 0));
    idleData.emplace_back(LEFT, "img/idle_up.png", Vec2(0.5, 0.92), Vec2(25, 0));
    idleData.emplace_back(UP, "img/idle_up.png", Vec2(0.5, 0.92), Vec2(25, 0));
    idleData.emplace_back(DOWN, "img/idle_down.png", Vec2(0.5, 0.92), Vec2(25, 0));


    closestNpcDistance = numeric_limits<float>::infinity();
    closestNpc = weak_ptr<GameObject>();
    shouldStopTalking = false;
}



Player::PlayerDirection Player::GetNewDirection(vector<PlayerDirection> directionsPressed) {
    for (auto &direction : directionsPressed) {
        if (direction == currentDirection) {
            return direction;
        }
    }

    return directionsPressed[0];
}

void Player::Update(float dt) {
        auto inputManager = InputManager::GetInstance();
        auto newState = state;
        auto collider = (Collider *)associated.GetComponent(COLLIDER_TYPE);
        lastPos = Vec2(associated.box.x, associated.box.y);

        if (state == TALKING && !shouldStopTalking) {
            newState = TALKING;
        } else if (inputManager.MousePress(RIGHT_MOUSE_BUTTON) || state == SHOOTING) {
            newState = SHOOTING;
        } else if (inputManager.MousePress(LEFT_MOUSE_BUTTON) || state == ATTACKING) {
            newState = ATTACKING;
        } else if (inputManager.IsKeyDown('a') || inputManager.IsKeyDown('d') || inputManager.IsKeyDown('s') || inputManager.IsKeyDown('w')) {
            newState = MOVING;
        } else {
            newState = IDLE;
        }

        
        if (shouldStopTalking && state != TALKING) {
            shouldStopTalking = false;
            auto collider = (Collider *)associated.GetComponent(COLLIDER_TYPE);
            collider->SetCanCollide([&] (GameObject &other) -> bool {
                return (other.HasComponent(NPC_TYPE) && InputManager::GetInstance().KeyPress(SPACE_KEY)) || other.HasComponent(COLLISION_MAP_TYPE);
            });
        }

        if (newState == TALKING && !closestNpc.expired()) {
            auto npc = (Npc *)closestNpc.lock()->GetComponent(NPC_TYPE);
            npc->Talk();
            
            speed = Vec2();
            collider->SetCanCollide([&] (GameObject &other) -> bool {
                return false;
            });
            closestNpcDistance = numeric_limits<float>::infinity();
            closestNpc = weak_ptr<GameObject>();
        } else if (newState == SHOOTING) {
            speed = Vec2();
            if (state != SHOOTING) {
                timer.Restart();
                Shoot();
            } else {
                timer.Update(dt);
                if (timer.Get() > BEAM_LIFETIME) {
                    newState = IDLE;
                }
            }
        } else if (newState == ATTACKING) {
            speed = Vec2();
            if (state != ATTACKING) {
                timer.Restart();
                Attack();
            } else {
                timer.Update(dt);
                if (timer.Get() > ATTACK_DURATION) {
                    newState = IDLE;
                }
            }
        } else if (newState == MOVING) {
            vector<PlayerDirection> directionsPressed;

            speed = Vec2();
            if (inputManager.IsKeyDown('a')) {
                directionsPressed.push_back(LEFT);
                speed += Vec2(-SPEED * dt, 0);
            }

            if (inputManager.IsKeyDown('d')) {
                directionsPressed.push_back(RIGHT);
                speed += Vec2(SPEED * dt, 0);
            }

            if (inputManager.IsKeyDown('w')) {
                directionsPressed.push_back(UP);
                speed += Vec2(0, -SPEED * dt);
            }

            if (inputManager.IsKeyDown('s')) {
                directionsPressed.push_back(DOWN);
                speed += Vec2(0, SPEED * dt);
            }

            auto oldDirection = currentDirection;
            currentDirection = GetNewDirection(directionsPressed);

            if (currentDirection != oldDirection) {
                ChangeDirection();
            }
        } else if (newState == IDLE) {
            speed = Vec2(0,0);
        }

        auto oldState = state;
        state = newState;

        if (state != oldState) {
            ChangeDirection();
        }

        associated.box += speed;
        //TODO: Find a way to make Collider update only after Player update
        collider->Update(0);
}

void Player::Render() {

}

bool Player::Is(string type) {
    return type == "Player";
}

void Player::Start() {
    Component::Start();
}

Player::~Player() {

}

void Player::NotifyCollision(GameObject &other) {
    if (other.HasComponent(NPC_TYPE)) {
        auto distance = (other.box.Center() - associated.box.Center()).Module();

        if (distance < closestNpcDistance) {
            closestNpcDistance = distance;
            closestNpc = Game::GetInstance().GetCurrentState().GetObjectPtr(&other);
            state = TALKING;
        }
    } else {
        auto collider = (Collider *)associated.GetComponent(COLLIDER_TYPE);
        associated.box -= speed;
        collider->Update(0);
    }
}

void Player::SetSprite(string file, int frameCount, float frameTime, bool flip) {
    auto sprite = (Sprite*)associated.GetComponent("Sprite");
    
    if(!sprite) {
        throw("Sprite component not found on Player's GameObject.");
    }
    
    sprite->SetFlip(flip);
    sprite->SetFrameCount(frameCount);
    sprite->SetFrameTime(frameTime);
    sprite->Open(file);
    sprite->SetFrame(0);
    
    associated.SetCenter(associated.box.Center());
}

void Player::Shoot() {
    auto& inputManager = InputManager::GetInstance();
    auto target = Vec2(inputManager.GetMouseX(), inputManager.GetMouseY());
    target = Camera::GetAbsolutePosition(associated.GetLayer(), target);

    currentDirection = GetNewDirection(target);

    auto playerBoxCenter = associated.box.Center();
    auto beamObj = new GameObject(associated.GetLayer());

    beamObj->box = Rect() + playerBoxCenter + GetStateData(shootingData).objectSpriteOffset;

    auto beamCpt = new BeamSkill(*beamObj, target);
    beamObj->AddComponent(beamCpt);
    Game::GetInstance().GetCurrentState().AddObject(beamObj);
}

void Player::Attack() {
    auto& inputManager = InputManager::GetInstance();
    auto target = Vec2(inputManager.GetMouseX(), inputManager.GetMouseY());
    target = Camera::GetAbsolutePosition(associated.GetLayer(), target);

    currentDirection = GetNewDirection(target);

    auto playerBoxCenter = associated.box.Center();
    auto attackObject = new GameObject(associated.GetLayer());

    attackObject->box = currentDirection == LEFT || currentDirection == RIGHT ? Rect(ATTACK_WIDTH, ATTACK_RANGE) : Rect(ATTACK_RANGE, ATTACK_WIDTH);
    attackObject->box.PlaceCenterAt(playerBoxCenter + GetStateData(attackingData).objectSpriteOffset);
    attackObject->AddComponent(new MeleeAttack(*attackObject));

    Game::GetInstance().GetCurrentState().AddObject(attackObject);
}

Player::PlayerDirection Player::GetNewDirection(Vec2 target) {
    auto angle = (target - associated.box.Center()).XAngleDeg();

    if (angle > -45 && angle < 45) {
        return RIGHT;
    } else if (angle > 45 && angle < 135) {
        return DOWN;
    } else if (angle > 135 || angle < -135) {
        return LEFT;
    } else {
        return UP;
    }
}

void Player::StopTalking() {
    shouldStopTalking = true;
}

bool Player::IsTalking() {
    return state == TALKING;
}

Player::PlayerStateData Player::GetStateData(vector<PlayerStateData> data) {
    for (auto &d : data) {
        if (d.direction == currentDirection) {
            return d;
        }
    }

    return data[0];
}

Player::PlayerStateData Player::ChangeDirection() {
    auto playerData = PlayerStateData(LEFT, "", Vec2(), Vec2());
    auto frameCount = WALK_SPRITE_COUNT;
    auto animationDuration = WALK_SPRITE_DURATION;
    auto shouldFlip = false;
    switch (state) {
        case MOVING:
            playerData = GetStateData(movingData);
            shouldFlip = currentDirection == LEFT;
            break;
        case ATTACKING:
            playerData = GetStateData(attackingData);
            frameCount = ATTACK_SPRITE_COUNT;
            animationDuration = ATTACK_DURATION;
            shouldFlip = currentDirection == LEFT;
            break;
        case SHOOTING:
            playerData = GetStateData(shootingData);
            frameCount = MAGIC_SPRITE_COUNT;
            animationDuration = BEAM_LIFETIME;
            shouldFlip = currentDirection == LEFT;
            break;
        default:
            playerData = GetStateData(idleData);
            frameCount = IDLE_SPRITE_COUNT;
            animationDuration = IDLE_SPRITE_DURATION;
    }
    auto collider = (Collider *) associated.GetComponent(COLLIDER_TYPE);
    collider->SetOffset(playerData.playerSpriteOffset);
    collider->SetScale(playerData.playerSpriteScale);
    SetSprite(playerData.animation, frameCount, animationDuration/frameCount, shouldFlip);

    return playerData;
}

Player::PlayerStateData::PlayerStateData(PlayerDirection direction,
                                         string animation,
                                         Vec2 scale,
                                         Vec2 offset,
                                         Vec2 objectSpriteDisplacement) : direction(direction),
                                                        animation(animation),
                                                        playerSpriteScale(scale),
                                                        playerSpriteOffset(offset), objectSpriteOffset(objectSpriteDisplacement) {
}
