//
// Created by fabio on 20/05/18.
//

#include <Sprite.h>
#include <InputManager.h>
#include <BeamSkill.h>
#include <Game.h>
#include "Player.h"

#define MAX_SPEED 20
#define SPEED 50
#define SPR_TIME 0.1

Player *Player::player = nullptr;

Player::Player(GameObject &associated) : Component(associated), speed({0, 0}), state(IDLE), hp(100) {
    Sprite *spr = new Sprite(associated, "img/idle1.png", 10, 0.1, 0, true);

    associated.AddComponent(spr);
    associated.box.h = spr->GetHeight();
    associated.box.w = spr->GetWidth();

    associated.SetCenter({associated.box.x, associated.box.y});

    Player::player = this;

    movementAnimations.emplace_back(make_pair(RIGHT, "img/run1.png"));
    movementAnimations.emplace_back(make_pair(LEFT, "img/run1.png"));
    movementAnimations.emplace_back(make_pair(UP, "img/walkTestUp.png"));
    movementAnimations.emplace_back(make_pair(DOWN, "img/walkTestDown.png"));

    shootingAnimations.emplace_back(make_pair(RIGHT, "img/magicTestSide.png"));
    shootingAnimations.emplace_back(make_pair(LEFT, "img/magicTestSide.png"));
    shootingAnimations.emplace_back(make_pair(UP, "img/magicTestUp.png"));
    shootingAnimations.emplace_back(make_pair(DOWN, "img/magicTestDown.png"));
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
    } else if (inputManager.IsKeyDown('a') || inputManager.IsKeyDown('d') || inputManager.IsKeyDown('s') || inputManager.IsKeyDown('w')) {
        newState = MOVING;
    } else {
        newState = IDLE;
    }
    
    if (newState == SHOOTING) {
        speed = Vec2();
        if (state != SHOOTING) {
            shootingTimer.Restart();
            Shoot();
        } else {
            shootingTimer.Update(dt);
            if (shootingTimer.Get() > BEAM_LIFETIME) {
                SetSprite("img/idle1.png", 10, 0.1);
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
            SetSprite(GetMovementAnimation(), 5, SPR_TIME, currentDirection == LEFT);
        }
    } else {
        if (state != IDLE) {
            SetSprite("img/idle1.png", 10, 0.1);
        }
        speed = Vec2(0,0);
    }

    state = newState;
    associated.box += speed;
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
    Component::NotifyCollision(other);
}

void Player::SetSprite(string file, int frameCount, float frameTime, bool flip) {
    auto sprite = (Sprite*)associated.GetComponent("Sprite");
    
    if(!sprite){
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

    auto angle = (target - associated.box.Center()).XAngleDeg();
    auto playerBoxPosition = Vec2(associated.box.x, associated.box.y);

    if (angle > -45 && angle < 45) {
        currentDirection = RIGHT;
    } else if (angle > 45 && angle < 135) {
        currentDirection = DOWN;
    } else if (angle > 135 || angle < -135) {
        currentDirection = LEFT;
    } else {
        currentDirection = UP;
    }

    SetSprite(GetShootingAnimation(), 6, BEAM_LIFETIME/6, currentDirection == RIGHT);

    auto beamObj = new GameObject(1);

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
