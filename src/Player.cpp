//
// Created by fabio on 20/05/18.
//

#include <Sprite.h>
#include <InputManager.h>
#include "Player.h"

#define MAX_SPEED 20
#define SPEED 50
#define SPR_TIME 0.1

Player *Player::player = nullptr;

void Player::Update(float dt) {
    auto inputManager = InputManager::GetInstance();
    auto center = associated.box.Center();   

    speed = Vec2();
    if (inputManager.IsKeyDown('a')) {
        if(state != MOVING_LEFT){
            state = MOVING_LEFT;
            if(inputManager.GetMouseX() >= center.x){
                SetSprite("img/run1.png", 5, SPR_TIME);
                lessMouseX = false;
            } else if(inputManager.GetMouseX() < center.x){
                SetSprite("img/run1.png", 5, SPR_TIME, true);
                lessMouseX = true;
            }
        } else {
            if(lessMouseX != false && inputManager.GetMouseX() >= center.x){
                SetSprite("img/run1.png", 5, SPR_TIME);
                lessMouseX = false;
            } else if(lessMouseX != true && inputManager.GetMouseX() < center.x){
                SetSprite("img/run1.png", 5, SPR_TIME, true);
                lessMouseX = true;
            }
        }
        speed = Vec2(-SPEED * dt, 0);
        
        if(inputManager.IsKeyDown('w')){
            speed += Vec2(0, -SPEED * dt);
        } else if(inputManager.IsKeyDown('s')){
            speed += Vec2(0, SPEED * dt);
        }
    } else if (inputManager.IsKeyDown('d')) {
        if(state != MOVING_RIGHT){
            state = MOVING_RIGHT;

            if(inputManager.GetMouseX() >= center.x){
                SetSprite("img/run1.png", 5, SPR_TIME);
                lessMouseX = false;
            } else if(inputManager.GetMouseX() < center.x){
                SetSprite("img/run1.png", 5, SPR_TIME, true);
                lessMouseX = true;
            }
        } else{

            if(lessMouseX != false && inputManager.GetMouseX() >= center.x){
                SetSprite("img/run1.png", 5, SPR_TIME);
                lessMouseX = false;
            } else if(lessMouseX != true && inputManager.GetMouseX() < center.x){
                SetSprite("img/run1.png", 5, SPR_TIME, true);
                lessMouseX = true;
            }
        }
        
        speed = Vec2(SPEED * dt, 0);
        if(inputManager.IsKeyDown('w')){
            speed += Vec2(0, -SPEED * dt);
        } else if(inputManager.IsKeyDown('s')){
            speed += Vec2(0, SPEED * dt);
        }
        
    } else if (inputManager.IsKeyDown('w')) {
        if(state != MOVING_UP){
            state = MOVING_UP; 
            if(inputManager.GetMouseY() >= center.y){
                SetSprite("img/walkTestDown.png", 5, SPR_TIME);
                lessMouseY = false;
            } else if(inputManager.GetMouseY() < center.y){
                SetSprite("img/walkTestUp.png", 5, SPR_TIME);
                lessMouseY = true;
            }
        } else{
            if(lessMouseY != false && inputManager.GetMouseY() >= center.y){
                SetSprite("img/walkTestDown.png", 5, SPR_TIME);
                lessMouseY = false;
            } else if(lessMouseY != true && inputManager.GetMouseY() < center.y){
                SetSprite("img/walkTestUp.png", 5, SPR_TIME);
                lessMouseY = true;
            }
        }
        speed = Vec2(0, -SPEED * dt);
    } else if (inputManager.IsKeyDown('s')) {
        if(state != MOVING_DOWN){
            state = MOVING_DOWN;
            if(inputManager.GetMouseY() >= center.y){
                SetSprite("img/walkTestDown.png", 5, SPR_TIME);
                lessMouseY = false;
            } else if(inputManager.GetMouseY() < center.y){
                SetSprite("img/walkTestUp.png", 5, SPR_TIME);
                lessMouseY = true;
            }
        } else{
            if(lessMouseY != false && inputManager.GetMouseY() >= center.y){
                SetSprite("img/walkTestDown.png", 5, SPR_TIME);
                lessMouseY = false;
            } else if(lessMouseY != true && inputManager.GetMouseY() < center.y){
                SetSprite("img/walkTestUp.png", 5, SPR_TIME);
                lessMouseY = true;
            }
        }
        speed = Vec2(0, SPEED * dt);
    } else{
        if(state != IDLE){
            SetSprite("img/idle1.png", 10, 0.1);
            state = IDLE;
            speed = Vec2(0,0);
            
            cout << "Pos X: " << associated.box.x << " Pos Y: " << associated.box.y << endl;
        }
    }
    
    associated.box += speed;
}

void Player::Render() {

}

bool Player::Is(string type) {
    return type == "Player";
}

Player::Player(GameObject &associated) : Component(associated), speed({0, 0}), state(IDLE), hp(100) {
    Sprite *spr = new Sprite(associated, "img/idle1.png", 10, 0.1, 0, true);

    associated.AddComponent(spr);
    associated.box.h = spr->GetHeight();
    associated.box.w = spr->GetWidth();

    associated.SetCenter({associated.box.x, associated.box.y});

    Player::player = this;

    //associated.AddComponent(new Collider(associated));
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
