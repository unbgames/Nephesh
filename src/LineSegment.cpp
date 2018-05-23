//
// Created by ftfnunes on 23/05/18.
//

#include "LineSegment.h"

LineSegment::LineSegment(Vec2 a, Vec2 b) : A(a), B(b) {
    tan = (b.x - a.x) / (b.y - a.y);
    lin = a.y - tan*a.x;
}

bool LineSegment::Contains(Vec2 dot) {
    return dot.x >= min(A.x, B.x) && dot.x <= max(A.x, B.x) && dot.y >= min(A.y, B.y) && dot.y <= max(A.y, B.y);
};

Vec2 LineSegment::GetIntersection(LineSegment seg) {
    auto intersectionX = (this->lin - seg.lin) / (seg.tan - this->tan);
    return Vec2(intersectionX, intersectionX*tan + lin);
}

bool LineSegment::operator==(LineSegment v2) {
    return this->tan == v2.tan && this->lin == v2.lin;
}

