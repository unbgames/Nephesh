//
// Created by ftfnunes on 23/05/18.
//

#include "LineSegment.h"

LineSegment::LineSegment(Vec2 dot1, Vec2 dot2) : dot1(dot1), dot2(dot2) {
    A = dot2.y - dot1.y;
    B = dot1.x - dot2.x;
    C = (A*dot1.x) + (B*dot1.y);
}

bool LineSegment::Contains(Vec2 dot) {
    return dot.x >= -1 + min(dot1.x, dot2.x) && dot.x <= 1 + max(dot1.x, dot2.x) && dot.y >= -1 + min(dot1.y, dot2.y) && dot.y <= 1 + max(dot1.y, dot2.y);
};

Vec2 LineSegment::GetIntersection(LineSegment seg) {
    double det = (A*seg.B) - (seg.A*B);

    if (det == 0) {
        return Vec2(-numeric_limits<float>::infinity(), -numeric_limits<float>::infinity());
    }

    auto x = ((seg.B * C) - (B * seg.C)) / det;
    auto y = ((A * seg.C) - (seg.A * C)) / det;
    return Vec2(x, y);
}

bool LineSegment::operator==(LineSegment seg) {
    return this->A == seg.A && this->B == seg.B && this->C == seg.C;
}

