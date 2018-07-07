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
#include <WorldState.h>
#include <Sound.h>
#include <IntervalTimer.h>
#include <TerrainMap.h>
#include <Charge.h>
#include "Player.h"

#define SPEED 200

Player *Player::player = nullptr;

Player::Player(GameObject &associated) : Component(associated), speed({0, 0}), state(STARTING), hp(100) {
    associated.AddComponent(new Sound(associated));
    Sprite *spr = new Sprite(associated, PLAYER_IDLE_SPRITE, PLAYER_IDLE_SPRITE_COUNT, 0.1, 0, true);

    associated.AddComponent(spr);

    associated.box.h = spr->GetHeight();
    associated.box.w = spr->GetWidth();

    associated.SetCenter({associated.box.x, associated.box.y});

    Player::player = this;

    movingData.emplace_back(RIGHT, "img/walk_side.png", Vec2(0.5, 0.92), Vec2(20, 0));
    movingData.emplace_back(LEFT, "img/walk_side.png", Vec2(0.5, 0.92), Vec2(-15, 0));
    movingData.emplace_back(UP, "img/walk_up.png", Vec2(0.4, 0.92), Vec2(0, 0));
    movingData.emplace_back(DOWN, "img/walk_down.png", Vec2(0.4, 0.92), Vec2(0, 0));
    grassStepSounds.emplace_back("audio/steps/grass/grass_1.wav");
    grassStepSounds.emplace_back("audio/steps/grass/grass_2.wav");
    grassStepSounds.emplace_back("audio/steps/grass/grass_3.wav");
    grassStepSounds.emplace_back("audio/steps/grass/grass_4.wav");
    grassStepSounds.emplace_back("audio/steps/grass/grass_5.wav");
    grassStepSounds.emplace_back("audio/steps/grass/grass_6.wav");
    stoneStepSounds.emplace_back("audio/steps/stone/stone_1.wav");
    stoneStepSounds.emplace_back("audio/steps/stone/stone_2.wav");
    stoneStepSounds.emplace_back("audio/steps/stone/stone_3.wav");
    stoneStepSounds.emplace_back("audio/steps/stone/stone_4.wav");
    stoneStepSounds.emplace_back("audio/steps/stone/stone_5.wav");
    stoneStepSounds.emplace_back("audio/steps/stone/stone_6.wav");
    dirtStepSounds.emplace_back("audio/steps/dirt/dirt_1.wav");
    dirtStepSounds.emplace_back("audio/steps/dirt/dirt_2.wav");
    dirtStepSounds.emplace_back("audio/steps/dirt/dirt_3.wav");
    dirtStepSounds.emplace_back("audio/steps/dirt/dirt_4.wav");
    dirtStepSounds.emplace_back("audio/steps/dirt/dirt_5.wav");
    dirtStepSounds.emplace_back("audio/steps/dirt/dirt_6.wav");


    dashingData.emplace_back(RIGHT, "img/dash_side.png", Vec2(0.5, 0.92), Vec2(20, 0));
    dashingData.emplace_back(LEFT, "img/dash_side.png", Vec2(0.5, 0.92), Vec2(-15, 0));
    dashingData.emplace_back(UP, "img/dash_up.png", Vec2(0.4, 0.92), Vec2(0, 0));
    dashingData.emplace_back(DOWN, "img/dash_down.png", Vec2(0.4, 0.92), Vec2(0, 0));
    dashSounds.emplace_back("audio/dash/dash_1.wav");
    dashSounds.emplace_back("audio/dash/dash_2.wav");
    dashSounds.emplace_back("audio/dash/dash_3.wav");

    shootingData.emplace_back(RIGHT, "img/magic_side.png", Vec2(0.5, 0.92), Vec2(20, 0), Vec2(65, -40));
    shootingData.emplace_back(LEFT, "img/magic_side.png", Vec2(0.5, 0.92), Vec2(-15, 0), Vec2(-60, -40));
    shootingData.emplace_back(UP, "img/magic_up.png", Vec2(0.4, 0.92), Vec2(0, 0), Vec2(3, -80));
    shootingData.emplace_back(DOWN, "img/magic_down.png", Vec2(0.4, 0.92), Vec2(0, 0), Vec2(4, 10));

    attackingData.emplace_back(RIGHT, "img/attack_side.png", Vec2(0.3, 0.92), Vec2(-10, 0), Vec2(60, 0));
    attackingData.emplace_back(LEFT, "img/attack_side.png", Vec2(0.3, 0.92), Vec2(10, 0), Vec2(-60, 0));
    attackingData.emplace_back(UP, "img/attack_up.png", Vec2(0.4, 0.92), Vec2(0, 0), Vec2(0, -40));
    attackingData.emplace_back(DOWN, "img/attack_down.png", Vec2(0.4, 0.92), Vec2(0, 0), Vec2(0, 30));
    attackMissSounds.emplace_back("audio/sword/attack_miss_1.wav");
    attackMissSounds.emplace_back("audio/sword/attack_miss_2.wav");
    attackMissSounds.emplace_back("audio/sword/attack_miss_3.wav");
    attackHitSounds.emplace_back("audio/sword/attack_hit_1.wav");
    attackHitSounds.emplace_back("audio/sword/attack_hit_2.wav");
    attackHitSounds.emplace_back("audio/sword/attack_hit_3.wav");

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
    if (!frozen) {
        auto inputManager = InputManager::GetInstance();
        auto newState = state;
        auto collider = (Collider *) associated.GetComponent(COLLIDER_TYPE);
        lastBox = collider->box;

        if (state != SHOOTING && state != ATTACKING && state != DASHING) {
            if (state == TALKING && !shouldStopTalking) {
                newState = TALKING;
            } else if (inputManager.MousePress(RIGHT_MOUSE_BUTTON) || state == SHOOTING) {
                newState = SHOOTING;
            } else if (inputManager.MousePress(LEFT_MOUSE_BUTTON) || state == ATTACKING) {
                newState = ATTACKING;
            } else if (inputManager.KeyPress(SHIFT_KEY) || state == DASHING) {
                newState = DASHING;
            } else if (inputManager.IsKeyDown('a') || inputManager.IsKeyDown('d') || inputManager.IsKeyDown('s') ||
                       inputManager.IsKeyDown('w')) {
                newState = MOVING;
            } else {
                newState = IDLE;
            }
        }

        if (shouldStopTalking && state != TALKING) {
            shouldStopTalking = false;
            auto collider = (Collider *) associated.GetComponent(COLLIDER_TYPE);
            collider->SetCanCollide([&](GameObject &other) -> bool {
                return (other.HasComponent(NPC_TYPE) && InputManager::GetInstance().KeyPress(SPACE_KEY)) ||
                       other.HasComponent(COLLISION_MAP_TYPE);
            });
        }

        if (newState != MOVING) {
            auto intervalTimer = (IntervalTimer *) associated.GetComponent(EVENT_TIMER_TYPE);
            if (intervalTimer != nullptr) {
                associated.RemoveComponent(intervalTimer);
            }
        }

        if (newState == TALKING && !closestNpc.expired()) {
            auto npc = (Npc *) closestNpc.lock()->GetComponent(NPC_TYPE);
            npc->Talk();

            speed = Vec2();
            collider->SetCanCollide([&](GameObject &other) -> bool {
                return false;
            });
            closestNpcDistance = numeric_limits<float>::infinity();
            closestNpc = weak_ptr<GameObject>();
        } else if (newState == DASHING) {
            speed = Vec2();
            if (state != DASHING) {
                //Start player dash animation
                timer.Restart();
                collider->SetCanCollide([&](GameObject &other) -> bool {
                    return other.HasComponent(COLLISION_MAP_TYPE);
                });
                Dash();
            } else {
                timer.Update(dt);

                if (timer.Get() > PLAYER_DASH_DURATION) {
                    collider->SetCanCollide([&] (GameObject &other) -> bool {
                        return (other.HasComponent(NPC_TYPE) && InputManager::GetInstance().KeyPress(SPACE_KEY)) || other.HasComponent(COLLISION_MAP_TYPE);
                    });
                    //Melee attack has finished, return player to IDLE state
                    newState = IDLE;
                } else {
                    auto d = target - collider->box.Center();
                    speed = Vec2(PLAYER_DASH_SPEED, 0).RotateDeg(d.XAngleDeg())*dt;
                }
            }

        } else if (newState == SHOOTING) {
            speed = Vec2();
            if (state != SHOOTING) {
                //Start the shooting animation
                preparing = true;
                target = Camera::GetAbsolutePosition(associated.GetLayer(),
                                                     Vec2(inputManager.GetMouseX(), inputManager.GetMouseY()));
                currentDirection = GetNewDirection(target);
                timer.Restart();
            } else {
                timer.Update(dt);
                if (preparing && timer.Get() > PLAYER_MAGIC_SPRITE_DURATION) {
                    //Player has finished preparing, lock the animation in the last frame and cast the magic.
                    Shoot();
                    preparing = false;
                    timer.Restart();
                    auto sprite = (Sprite *) associated.GetComponent(SPRITE_TYPE);
                    sprite->LockFrame();
                } else if (!preparing && timer.Get() > BEAM_LIFETIME + CHARGING_DURATION) {
                    //Player magic has finished, return player to IDLE state
                    newState = IDLE;
                    auto sprite = (Sprite *) associated.GetComponent(SPRITE_TYPE);
                    sprite->UnlockFrame();
                }
            }
        } else if (newState == ATTACKING) {
            speed = Vec2();
            if (state != ATTACKING) {
                //Start player attacking animation and create MeleeAttack object
                timer.Restart();
                attacked = false;
                Attack();
            } else {
                timer.Update(dt);
                if (!attacked) {
                    auto attack = (MeleeAttack *) meleeAttack.lock()->GetComponent(MELEE_ATTACK_TYPE);
                    attacked = true;
                    if (!attack->AttackHit()) {
                        PlaySound(attackMissSounds[rand() % attackMissSounds.size()]);
                    } else {
                        PlaySound(attackHitSounds[rand() % attackHitSounds.size()]);
                    }
                }

                if (timer.Get() > PLAYER_ATTACK_DURATION) {
                    //Melee attack has finished, return player to IDLE state
                    newState = IDLE;
                }
            }

        } else if (newState == MOVING) {

            if (state != MOVING) {
                associated.AddComponent(new IntervalTimer(associated, PLAYER_STEP_INTERVAL, [&]() -> void {
                    PlaySound(GetRandomStepSound());
                }));
            }
            vector<PlayerDirection> directionsPressed;
            speed = Vec2();

            //Add movement vectors according to the pressed keys, also, store which keys were pressed
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

            //Get new direction based on keys pressed, the selection is as follow
            // if the key corresponding to the current direction was pressed, keep the current direction
            // else choose the direction of the first key pressed

            currentDirection = GetNewDirection(directionsPressed);

            if (currentDirection != oldDirection) {
                //if the direction changed, force an sprite direction update
                ChangeDirection();
            }
        } else if (newState == IDLE) {
            speed = Vec2(0, 0);
        }

        auto oldState = state;
        state = newState;

        if (state != oldState) {
            ChangeDirection();
        }

        associated.box += speed;
    } else {
        state = IDLE;

        ChangeDirection();
    }
}

void Player::Render() {

}

bool Player::Is(string type) {
    return type == PLAYER_TYPE;
}

void Player::Start() {
    auto collider = new Collider(associated, Vec2(0.5, 0.92));
    collider->SetCanCollide([&] (GameObject &other) -> bool {
        return (other.HasComponent(NPC_TYPE) && InputManager::GetInstance().KeyPress(SPACE_KEY)) || other.HasComponent(COLLISION_MAP_TYPE);
    });

    associated.AddComponent(collider);
    auto &state = (WorldState &) Game::GetInstance().GetCurrentState();
    state.AddCollider(state.GetObjectPtr(&associated).lock());
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
            ChangeDirection();
        }
    } else {
        auto collider = (Collider *)associated.GetComponent(COLLIDER_TYPE);
        auto collidable = (Collidable *)other.GetComponent(COLLIDABLE_TYPE);
        auto intersections = collidable->GetIntersections(*collider);
        auto boxCorners = collider->box.GetCorners(associated.angleDeg, associated.rotationCenter);

        auto u = LineSegment(boxCorners[0], boxCorners[1]);
        auto r = LineSegment(boxCorners[1], boxCorners[2]);
        auto d = LineSegment(boxCorners[2], boxCorners[3]);
        auto l = LineSegment(boxCorners[3], boxCorners[0]);

        auto lCount = 0;
        auto rCount = 0;
        auto uCount = 0;
        auto dCount = 0;

        auto sMinX = std::numeric_limits<float>::infinity();
        auto sMinY = std::numeric_limits<float>::infinity();
        auto sMaxX = -std::numeric_limits<float>::infinity();
        auto sMaxY = -std::numeric_limits<float>::infinity();
        auto fMinX = std::numeric_limits<float>::infinity();
        auto fMinY = std::numeric_limits<float>::infinity();
        auto fMaxX = -std::numeric_limits<float>::infinity();
        auto fMaxY = -std::numeric_limits<float>::infinity();
        auto absoluteMinX = std::numeric_limits<float>::infinity();
        auto absoluteMinY = std::numeric_limits<float>::infinity();
        auto absoluteMaxX = -std::numeric_limits<float>::infinity();
        auto absoluteMaxY = -std::numeric_limits<float>::infinity();
        auto currentMinX = collider->box.x;
        auto currentMaxX = collider->box.x + collider->box.w;
        auto currentMinY = collider->box.y;
        auto currentMaxY = collider->box.y + collider->box.h;
        vector<Intersection> verifyiedLines;

        for (auto &intersection : intersections) {
            auto colliderLine = intersection.colliderLine;
            auto collidableLine = intersection.collidableLine;
            if (intersection.intersectionPoint.x == -numeric_limits<float>::infinity()) {
                continue;
            }
            auto isVerifyied = false;
            for (auto &lines : verifyiedLines) {
                if (colliderLine == lines.colliderLine && collidableLine == lines.collidableLine) {
                    isVerifyied = true;
                }
            }
            if (isVerifyied) {
                continue;
            } else {
                verifyiedLines.push_back(intersection);
            }

            vector<Vec2> dots { collidableLine.dot1, collidableLine.dot2 };
            float *minX = &sMinX;
            float *maxX = &sMaxX;
            float *minY = &sMinY;
            float *maxY = &sMaxY;

            if (colliderLine == u || colliderLine == d) {
                minX = &fMinX;
                maxX = &fMaxX;
                minY = &fMinY;
                maxY = &fMaxY;
            }
            
            if (colliderLine == u) { uCount++; }
            if (colliderLine == d) { dCount++; }
            if (colliderLine == l) { lCount++; }
            if (colliderLine == r) { rCount++; }

            for (auto &dot : dots) {
                if (dot.x < absoluteMinX) {
                    absoluteMinX = dot.x;
                }
                if (dot.x > currentMinX && dot.x < currentMaxX && dot.x < *minX) {
                    *minX = dot.x;
                }
                if (dot.x > absoluteMaxX) {
                    absoluteMaxX = dot.x;
                }
                if (dot.x > currentMinX && dot.x < currentMaxX && dot.x > *maxX) {
                    *maxX = dot.x;
                }
                if (dot.y > absoluteMaxY) {
                    absoluteMaxY = dot.y;
                }
                if (dot.y > currentMinY && dot.y < currentMaxY && dot.y < *minY) {
                    *minY = dot.y;
                }
                if (dot.y < absoluteMinY) {
                    absoluteMinY = dot.y;
                }
                if (dot.y > currentMinY && dot.y < currentMaxY && dot.y > *maxY) {
                    *maxY = dot.y;
                }
            }
        }

        auto playerMinX = lastBox.x;
        auto playerMaxX = lastBox.x + lastBox.w;
        auto playerMinY = lastBox.y;
        auto playerMaxY = lastBox.y + lastBox.h;

        if (dCount >= 1 && uCount >= 1) {
            if (speed.x > 0) {
                collider->box.x = fMaxX - collider->box.w - 1;
            } else if (speed.x < 0) {
                collider->box.x = fMinX + 1;
            }
        } else if (rCount > 1 || lCount > 1 || (((rCount > 0 && lCount == 0 )  || (lCount > 0 && rCount == 0)) && (playerMinX > absoluteMaxX || playerMaxX < absoluteMinX))) {
            if (speed.x > 0) {
                collider->box.x = sMinX - collider->box.w - 1;
            } else if (speed.x < 0){
                collider->box.x = sMaxX + 1;
            }
        }
        
        if (rCount >= 1 && lCount >= 1) {
            if (speed.y > 0) {
                collider->box.y = sMaxY - collider->box.h - 1;
            } else if (speed.y < 0) {
                collider->box.y = sMinY + 1;
            }
        } else if (uCount > 1 || dCount > 1 || (((dCount > 0 && uCount == 0 )  || (uCount > 0 && dCount == 0)) && (playerMinY > absoluteMaxY || playerMaxY < absoluteMinY))) {
            if (speed.y > 0) {
                collider->box.y = fMinY - collider->box.h - 1;
            } else if (speed.y < 0){
                collider->box.y = fMaxY + 1;
            }
        }

        collider->UpdateGameObject();
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
    sprite->Open(file, false);
    sprite->SetFrame(0);

    //associated.SetCenter(associated.box.Center());
}

void Player::Shoot() {
    auto playerBoxCenter = associated.box.Center();
    string spriteName = "";
    Vec2 offset;

    switch (currentDirection) {
        case RIGHT:
        case LEFT:
            spriteName = "img/pre_magic_side.png";
            break;
        case UP:
            offset = Vec2(-24, 0);
            spriteName = "img/pre_magic_up.png";
            break;
        case DOWN:
            offset = Vec2(-17, 0);
            spriteName = "img/pre_magic_down.png";
            break;
    }

    auto beamObj = new GameObject(associated.GetLayer());
    beamObj->box = Rect() + playerBoxCenter + GetStateData(shootingData).objectSpriteOffset;
    beamObj->box += offset;
    auto beamCpt = new BeamSkill(*beamObj, target, currentDirection);
    beamObj->AddComponent(beamCpt);

    auto chargeObj = new GameObject(associated.GetLayer());
    chargeObj->AddComponent(new Charge(*chargeObj, beamObj, CHARGING_DURATION));
    auto raySprite = new Sprite(*chargeObj, spriteName, 4, CHARGING_DURATION/4, 0, false, currentDirection == LEFT);
    chargeObj->AddComponent(raySprite);
    chargeObj->box = Rect() + playerBoxCenter + GetStateData(shootingData).objectSpriteOffset;
    chargeObj->box += offset;
    Game::GetInstance().GetCurrentState().AddObject(chargeObj);
    PlaySound("audio/magic_attack.wav");
}

void Player::Attack() {
    auto& inputManager = InputManager::GetInstance();
    auto target = Vec2(inputManager.GetMouseX(), inputManager.GetMouseY());
    target = Camera::GetAbsolutePosition(associated.GetLayer(), target);

    currentDirection = GetNewDirection(target);

    auto playerBoxCenter = associated.box.Center();
    auto attackObject = new GameObject(associated.GetLayer());

    attackObject->box = currentDirection == LEFT || currentDirection == RIGHT ? Rect(PLAYER_ATTACK_WIDTH, PLAYER_ATTACK_RANGE) : Rect(PLAYER_ATTACK_RANGE, PLAYER_ATTACK_WIDTH);
    attackObject->box.PlaceCenterAt(playerBoxCenter + GetStateData(attackingData).objectSpriteOffset);
    attackObject->AddComponent(new MeleeAttack(*attackObject));

    meleeAttack = Game::GetInstance().GetCurrentState().AddObject(attackObject);
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
    auto frameCount = PLAYER_WALK_SPRITE_COUNT;
    auto animationDuration = PLAYER_WALK_SPRITE_DURATION;
    auto shouldFlip = false;
    switch (state) {
        case MOVING:
            playerData = GetStateData(movingData);
            shouldFlip = currentDirection == LEFT;
            break;
        case ATTACKING:
            playerData = GetStateData(attackingData);
            frameCount = PLAYER_ATTACK_SPRITE_COUNT;
            animationDuration = PLAYER_ATTACK_DURATION;
            shouldFlip = currentDirection == LEFT;
            break;
        case DASHING:
            playerData = GetStateData(dashingData);
            frameCount = PLAYER_DASH_SPRITE_COUNT;
            animationDuration = PLAYER_DASH_DURATION;
            shouldFlip = currentDirection == LEFT;
            break;
        case SHOOTING:
            playerData = GetStateData(shootingData);
            frameCount = PLAYER_MAGIC_SPRITE_COUNT;
            animationDuration = PLAYER_MAGIC_SPRITE_DURATION;
            shouldFlip = currentDirection == LEFT;
            break;
        default:
            playerData = GetStateData(idleData);
            frameCount = PLAYER_IDLE_SPRITE_COUNT;
            animationDuration = PLAYER_IDLE_SPRITE_DURATION;
    }
    auto collider = (Collider *) associated.GetComponent(COLLIDER_TYPE);
    collider->SetOffset(playerData.playerSpriteOffset);
    collider->SetScale(playerData.playerSpriteScale);
    SetSprite(playerData.animation, frameCount, animationDuration/frameCount, shouldFlip);
    if (collider->box.w != 0) {
        collider->UpdateGameObject();
    }

    return playerData;
}

void Player::Dash() {
    auto &inputManager = InputManager::GetInstance();
    auto mousePos = inputManager.GetMouse() + Camera::pos;

    currentDirection = GetNewDirection(mousePos);

    target = associated.box.Center() + Vec2(PLAYER_DASH_SPEED*PLAYER_DASH_DURATION, 0).RotateDeg((mousePos - associated.box.Center()).XAngleDeg());
    PlaySound(dashSounds[rand()%dashSounds.size()]);
}

Vec2 Player::GetCenter() {
    return associated.box.Center();
}

void Player::PlaySound(string file) {
    auto sound = (Sound *) associated.GetComponent(SOUND_TYPE);
    sound->Open(file);
    sound->Play();
}

string Player::GetRandomStepSound() {
    auto &map = ((WorldState &)Game::GetInstance().GetCurrentState()).GetCurrentMap();

    auto terrainMap = (TerrainMap *) map.GetTileMap()->GetComponent(TERRAIN_MAP_TYPE);
    if (terrainMap != nullptr) {
        auto terrain = terrainMap->GetCurrentTerrain(associated.box.Center() + Vec2(0, associated.box.h/2), associated.GetLayer());

        switch (terrain) {
            case TerrainMap::TerrainType::DIRT:
                return dirtStepSounds[rand()%dirtStepSounds.size()];
            case TerrainMap::TerrainType::STONE:
                return stoneStepSounds[rand()%stoneStepSounds.size()];
            case TerrainMap::TerrainType::GRASS:
                return grassStepSounds[rand()%grassStepSounds.size()];
        }
    }

    return dirtStepSounds[rand()%dirtStepSounds.size()];
}

void Player::Freeze() {
    frozen = true;
}

void Player::Unfreeze() {
    frozen = false;
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
