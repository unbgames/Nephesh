//
// Created by ftfnunes on 23/05/18.
//

#ifndef NEPHESH_LINESEGMENT_H
#define NEPHESH_LINESEGMENT_H


#include "Vec2.h"
#include <algorithm>

using namespace std;

class LineSegment {
public:
    Vec2 A;
    Vec2 B;
    float tan;
    float lin;

    LineSegment(Vec2 a, Vec2 b);

    bool Contains(Vec2 dot);

    Vec2 GetIntersection(LineSegment seg);

    bool operator==(LineSegment v2);
};


#endif //NEPHESH_LINESEGMENT_H
