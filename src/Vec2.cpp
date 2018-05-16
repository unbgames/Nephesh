#include <cmath>
#include "Vec2.h"

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2 Vec2::operator+(const Vec2 v2) {
    return { x + v2.x, y + v2.y };
}

Vec2 Vec2::operator-(const Vec2 v2) {
    return { x - v2.x, y - v2.y };
}

Vec2 Vec2::operator*(const float scalar) {
    return { x*scalar, y*scalar };
}

float Vec2::Module() {
    return sqrt(x*x + y*y);
}

Vec2 Vec2::Normalize() {
    float module = Module();
    return {x/module, y/module};
}

float Vec2::Distance(Vec2 target) {
    return (*this - target).Module();
}

float Vec2::XAngle() {
    return atan2(y, x);
}

float Vec2::DiferenceAngle(Vec2 target) {
    return (target - *this).XAngle();
}

Vec2 Vec2::Rotate(float angle) {
    float cosO = cos(angle);
    float sinO = sin(angle);
    return { x*cosO - y*sinO, x*sinO + y*cosO };
}

Vec2 Vec2::RotateDeg(float angle) {
    auto rad = 2*M_PI*(angle/360.0);
    float cosO = cos(rad);
    float sinO = sin(rad);
    return { x*cosO - y*sinO, x*sinO + y*cosO };
}

float Vec2::XAngleDeg() {
    return (XAngle()/(2*M_PI))*360;
}

void Vec2::operator+=(Vec2 v) {
    *this = *this + v;
}


void Vec2::operator-=(Vec2 v) {
    *this = *this - v;
}

void Vec2::operator*=(float scalar) {
    *this = *this * scalar;
}



