//
// Created by fabio on 20/05/18.
//

#include <Sprite.h>
#include <InputManager.h>
#include <BeamSkill.h>
#include <Game.h>
#include <Collider.h>
#include <MeleeAttack.h>
#include "Player.h"

#define MAX_SPEED 20
#define SPEED 50
#define SPR_TIME 0.1

Player *Player::player = nullptr;

Player::Player(GameObject &associated) : Component(associated), speed({0, 0}), state(IDLE), hp(100) {
    Sprite *spr = new Sprite(associated, "img/idle.png", IDLE_SPRITE_COUNT, 0.1, 0, true);

    associated.AddComponent(spr);
    associated.box.h = spr->GetHeight();
    associated.box.w = spr->GetWidth();

    associated.SetCenter({associated.box.x, associated.box.y});

    Player::player = this;

    movementAnimations.emplace_back(make_pair(RIGHT, "img/walk_side.png"));
    movementAnimations.emplace_back(make_pair(LEFT, "img/walk_side.png"));
    movementAnimations.emplace_back(make_pair(UP, "img/walk_up.png"));
    movementAnimations.emplace_back(make_pair(DOWN, "img/walk_down.png"));

    shootingAnimations.emplace_back(make_pair(RIGHT, "img/magic_side.png"));
    shootingAnimations.emplace_back(make_pair(LEFT, "img/magic_side.png"));
    shootingAnimations.emplace_back(make_pair(UP, "img/magic_up.png"));
    shootingAnimations.emplace_back(make_pair(DOWN, "img/magic_down.png"));

    attackAnimations.emplace_back(make_pair(RIGHT, "img/attack_side.png"));
    attackAnimations.emplace_back(make_pair(LEFT, "img/attack_side.png"));
    attackAnimations.emplace_back(make_pair(UP, "img/attack_up.png"));
    attackAnimations.emplace_back(make_pair(DOWN, "img/attack_down.png"));
}

string Player::GetMovementAnimation() {
    for (auto &animation : movementAnimations) {
        if (animation.first == currentDirection) {
            return animation.second;
        }
    }

    return nullptr;
}

string Player::GetShootingAnimation() {
    for (auto &animation : shootingAnimations) {
        if (animation.first == currentDirection) {
            return animation.second;
        }
    }

    return nullptr;
}


string Player::GetAttackAnimation() {
    for (auto &animation : attackAnimations) {
        if (animation.first == currentDirection) {
            return animation.second;
        }
    }

    return nullptr;
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

    if (inputManager.MousePress(LEFT_MOUSE_BUTTON) || state == SHOOTING) {
        newState = SHOOTING;
    } else if (inputManager.KeyPress(SPACE_KEY) || state == ATTACKING) {
        newState = ATTACKING;
    } else if (inputManager.IsKeyDown('a') || inputManager.IsKeyDown('d') || inputManager.IsKeyDown('s') || inputManager.IsKeyDown('w')) {
        newState = MOVING;
    } else {
        newState = IDLE;
    }
    
    if (newState == SHOOTING) {
        speed = Vec2();
        if (state != SHOOTING) {
            timer.Restart();
            Shoot();
        } else {
            timer.Update(dt);
            if (timer.Get() > BEAM_LIFETIME) {
                SetSprite("img/idle.png", MAGIC_SPRITE_COUNT, 0.1);
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
                SetSprite("img/idle.png", MAGIC_SPRITE_COUNT, 0.1);
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
        
        if (state != MOVING || currentDirection != oldDirection) {
            SetSprite(GetMovementAnimation(), WALK_SPRITE_COUNT, SPR_TIME, currentDirection == LEFT);
        }
    } else {
        if (state != IDLE) {
            SetSprite("img/idle.png", IDLE_SPRITE_COUNT, 0.1);
        }
        speed = Vec2(0,0);
    }

    state = newState;
    associated.box += speed;
}

void Player::Render() {

}

bool Player::Is(string type) {
    return type == PLAYER_TYPE;
}

void Player::Start() {
    Component::Start();
}

Player::~Player() {

}

void Player::NotifyCollision(GameObject &other) {
    Component::NotifyCollision(other);
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

    currentDirection = GetDirection(target);

    SetSprite(GetShootingAnimation(), MAGIC_SPRITE_COUNT, BEAM_LIFETIME/MAGIC_SPRITE_COUNT, currentDirection == LEFT);

    auto playerBoxPosition = Vec2(associated.box.x, associated.box.y);
    auto beamObj = new GameObject(associated.GetLayer());

    if (currentDirection == RIGHT) {
        beamObj->box = Rect() + playerBoxPosition + Vec2(associated.box.w, associated.box.h/2 );
    } else if (currentDirection == DOWN) {
        beamObj->box = Rect() + playerBoxPosition + Vec2(associated.box.w/2, associated.box.h);
    } else if (currentDirection == LEFT) {
        beamObj->box = Rect() + playerBoxPosition + Vec2(0, associated.box.h/2);
    } else {
        beamObj->box = Rect() + playerBoxPosition + Vec2(associated.box.w/2, 0);
    }

    auto beamCpt = new BeamSkill(*beamObj, target);
    beamObj->AddComponent(beamCpt);
    Game::GetInstance().GetCurrentState().AddObject(beamObj);
}

void Player::Attack() {
    auto& inputManager = InputManager::GetInstance();
    auto target = Vec2(inputManager.GetMouseX(), inputManager.GetMouseY());

    currentDirection = GetDirection(target);

    SetSprite(GetAttackAnimation(), ATTACK_SPRITE_COUNT, ATTACK_DURATION/ATTACK_SPRITE_COUNT, currentDirection == LEFT);

    auto attackObject = new GameObject(associated.GetLayer());
    attackObject->AddComponent(new MeleeAttack(*attackObject));
    auto collider = (Collider *) attackObject->GetComponent(COLLIDER_TYPE);
    auto playerBoxPosition = Vec2(associated.box.x, associated.box.y);
    auto playerBoxCenter = associated.box.Center();

    if (currentDirection == RIGHT) {
        collider->SetOffset(Vec2(-35, 0));
        attackObject->box = Rect(ATTACK_WIDTH, ATTACK_RANGE);
        attackObject->box.PlaceCenterAt(playerBoxPosition + Vec2(associated.box.w + attackObject->box.w/2, associated.box.h/2));
    } else if (currentDirection == DOWN) {
        collider->SetOffset(Vec2(0, -30));
        attackObject->box = Rect(ATTACK_RANGE, ATTACK_WIDTH);
        attackObject->box.PlaceCenterAt(playerBoxPosition + Vec2(associated.box.w/2, associated.box.h + attackObject->box.h/2));
    } else if (currentDirection == LEFT) {
        collider->SetOffset(Vec2(50, 0));
        attackObject->box = Rect(ATTACK_WIDTH, ATTACK_RANGE);
        attackObject->box.PlaceCenterAt(playerBoxPosition + Vec2(-attackObject->box.w, associated.box.h/2));
    } else {
        collider->SetOffset(Vec2(0, 40));
        attackObject->box = Rect(ATTACK_RANGE, ATTACK_WIDTH);
        attackObject->box.PlaceCenterAt(playerBoxPosition + Vec2(associated.box.w/2, -attackObject->box.h));
    }

    Game::GetInstance().GetCurrentState().AddObject(attackObject);
}

Player::PlayerDirection Player::GetDirection(Vec2 target) {
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

Vec2 Player::GetCenter() {
    return associated.box.Center();
}
