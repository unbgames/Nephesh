//
// Created by ftfnunes on 17/04/18.
//

#include <SDL_rect.h>
#include <Camera.h>
#include <Game.h>
#include "Collider.h"
#ifdef DEBUG
#include "Camera.h"
#include "Game.h"

#include <SDL2/SDL.h>
#endif // DEBUG


Collider::Collider(GameObject &associated, Vec2 scale, Vec2 offset) : Component(associated),
                                                                      scale(scale),
                                                                      offset(offset) {
}

void Collider::Update(float dt) {
    auto newBox = Rect();
    newBox.w = associated.box.w*scale.x;
    newBox.h = associated.box.h*scale.y;
    newBox.x = associated.box.Center().x - newBox.w/2;
    newBox.y = associated.box.Center().y - newBox.h/2;

    box = newBox + offset.RotateDeg(associated.angleDeg);
}

void Collider::Render() {
#ifdef DEBUG
    Vec2 center( box.Center() );
	SDL_Point points[5];

	Vec2 point = (Vec2(box.x, box.y) - box.Center()).RotateDeg(associated.angleDeg) + box.Center();
    point = Camera::GetRenderPosition(associated.GetLayer(), point);
	points[0] = {(int)point.x, (int)point.y};
	points[4] = {(int)point.x, (int)point.y};

    point = (Vec2(box.x + box.w, box.y) - box.Center()).RotateDeg(associated.angleDeg) + box.Center();
	point = Camera::GetRenderPosition(associated.GetLayer(), point);
	points[1] = {(int)point.x, (int)point.y};

    point = (Vec2(box.x + box.w, box.y + box.h) - box.Center()).RotateDeg(associated.angleDeg) + box.Center();
	point = Camera::GetRenderPosition(associated.GetLayer(), point);
	points[2] = {(int)point.x, (int)point.y};

    point = (Vec2(box.x, box.y + box.h) - box.Center()).RotateDeg(associated.angleDeg) + box.Center();
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

