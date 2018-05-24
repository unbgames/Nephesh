//
// Created by bruno on 22/05/18.
//

#include <algorithm>
#include <math.h>
#include "Collidable.h"

#ifdef DEBUG
#define INCLUDE_SDL
#include "SDL_include.h"
#include "Camera.h"
#include "Game.h"
#endif // DEBUG


Collidable::Collidable(GameObject &associated, Vec2 scale, Vec2 offset):
        Component(associated),
        scale(scale),
        offset(offset) {

}

void Collidable::Update(float dt) {
    auto newBox = Rect();
    newBox.w = associated.box.w*scale.x;
    newBox.h = associated.box.h*scale.y;
    newBox.PlaceCenterAt(associated.box.Center());

    box = newBox + offset.RotateDeg(associated.angleDeg);
}

void Collidable::Render() {
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

bool Collidable::Is(string type) {
    return type == COLLIDABLE_TYPE;
}

bool Collidable::IsColliding(Collider& collider) {
    auto angleOfCollidable = (float)(associated.angleDeg * M_PI/180);
    auto angleOfCollider = (float)(collider.GetGameObject().angleDeg * M_PI/180);

    auto collidableBox = box;
    auto colliderBox = collider.box;

    // get the unrotated vertices of the collider box (the vertices from the Rect itself)
    Vec2 A[] = { Vec2( collidableBox.x, collidableBox.y + collidableBox.h ),
                 Vec2( collidableBox.x + collidableBox.w, collidableBox.y + collidableBox.h ),
                 Vec2( collidableBox.x + collidableBox.w, collidableBox.y ),
                 Vec2( collidableBox.x, collidableBox.y )
    };
    Vec2 B[] = { Vec2( colliderBox.x, colliderBox.y + colliderBox.h ),
                 Vec2( colliderBox.x + colliderBox.w, colliderBox.y + colliderBox.h ),
                 Vec2( colliderBox.x + colliderBox.w, colliderBox.y ),
                 Vec2( colliderBox.x, colliderBox.y )
    };

    // get the rotated vertices of the box, according to angleOfCollider (angleDeg)
    for (auto& v : A) {
        v = (v - collidableBox.Center()).Rotate(angleOfCollidable) + collidableBox.Center();
    }

    for (auto& v : B) {
        v = (v - colliderBox.Center()).Rotate(angleOfCollider) + colliderBox.Center();
    }

    Vec2 axes[] = { (A[0] - A[1]).Normalize(), (A[1] - A[2]).Normalize(), (B[0] - B[1]).Normalize(), (B[1] - B[2]).Normalize() };

    for (auto& axis : axes) {
        float P[4];

        for (int i = 0; i < 4; ++i) P[i] = A[i] * axis;

        float minA = *std::min_element(P, P + 4);
        float maxA = *std::max_element(P, P + 4);

        for (int i = 0; i < 4; ++i) P[i] = B[i] * axis;

        float minB = *std::min_element(P, P + 4);
        float maxB = *std::max_element(P, P + 4);

        if (maxA < minB || minA > maxB)
            return false;
    }

    return true;
}

void Collidable::SetScale(Vec2 scale) {
    this->scale = scale;
}

void Collidable::SetOffset(Vec2 offset) {
    this->offset = offset;
}
