//
// Created by ftfnunes on 17/04/18.
//

#include <Camera.h>
#include <Game.h>
#include "Collider.h"
#include <utility>
#ifdef DEBUG
#include "Camera.h"
#include "Game.h"

#include <SDL2/SDL.h>
#include <LineSegment.h>

#endif // DEBUG


Collider::Collider(GameObject &associated, Vec2 scale, Vec2 offset) : Component(associated),
                                                                      scale(scale),
                                                                      offset(offset) {
    // default: return true
    canCollide = [] (GameObject& collidable) -> bool {
        return true;
    };
}

void Collider::Update(float dt) {
    auto newBox = Rect();
    newBox.w = associated.box.w*scale.x;
    newBox.h = associated.box.h*scale.y;
    newBox.PlaceCenterAt(associated.box.Center());

    box = newBox + offset.RotateDeg(associated.angleDeg);
}

void Collider::Render() {
#ifdef DEBUG
    auto center = Vec2(box.x, box.y) + associated.rotationCenter;
	SDL_Point points[5];

	Vec2 point = (Vec2(box.x, box.y) - center).RotateDeg(associated.angleDeg) + center;
    point = Camera::GetRenderPosition(associated.GetLayer(), point);
	points[0] = {(int)point.x, (int)point.y};
	points[4] = {(int)point.x, (int)point.y};

    point = (Vec2(box.x + box.w, box.y) - center).RotateDeg(associated.angleDeg) + center;
	point = Camera::GetRenderPosition(associated.GetLayer(), point);
	points[1] = {(int)point.x, (int)point.y};

    point = (Vec2(box.x + box.w, box.y + box.h) - center).RotateDeg(associated.angleDeg) + center;
	point = Camera::GetRenderPosition(associated.GetLayer(), point);
	points[2] = {(int)point.x, (int)point.y};

    point = (Vec2(box.x, box.y + box.h) - center).RotateDeg(associated.angleDeg) + center;
	point = Camera::GetRenderPosition(associated.GetLayer(), point);
	points[3] = {(int)point.x, (int)point.y};

	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif
}

bool Collider::Is(string type) {
    return type == COLLIDER_TYPE;
}

void Collider::SetScale(Vec2 scale) {
    this->scale = scale;
}

void Collider::SetOffset(Vec2 offset) {
    this->offset = offset;
}

void Collider::SetCanCollide(function<bool(GameObject &collidable)> canCollide) {
    this->canCollide = canCollide;
}

bool Collider::CanCollide(GameObject &collidable) {
    return canCollide(collidable);
}

Vec2 Collider::GetOffset() {
    return offset;
}

void Collider::UpdateGameObject() {
    auto newBox = Rect(associated.box.h, associated.box.w);
    newBox.PlaceCenterAt(box.Center() - offset);
    //TODO: Add rotation if needed
    associated.box = newBox;
}

Vec2 Collider::GetScale() {
    return scale;
}
