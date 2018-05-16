#include "Rect.h"

Rect::Rect() : x(0), y(0), h(0), w(0) {}
Rect::Rect(float h, float w) : x(0), y(0), h(h), w(w) {}
Rect::Rect(float x, float y, float h, float w) : x(x), y(y), h(h), w(w) {}

Rect Rect::operator+(const Vec2 displacement) {
    return { x + displacement.x, y + displacement.y, h, w };
}

void Rect::operator+=(Vec2 v2) {
    this->x += v2.x;
    this->y += v2.y;
}

Vec2 Rect::Center() {
    return { x + (w/2), y + (h/2) };
}

bool Rect::Contains(Vec2 dot) {
    return dot.x > x && dot.x < x+w && dot.y > y && dot.y < y+h;
}

float Rect::DistanceFrom(Rect target) {
    return Center().Distance(target.Center());
}

