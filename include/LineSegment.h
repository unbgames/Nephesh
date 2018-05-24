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
    Vec2 dot1;
    Vec2 dot2;
    double A;
    double B;
    double C;

    LineSegment(Vec2 dot1, Vec2 dot2);

    bool Contains(Vec2 dot);

    Vec2 GetIntersection(LineSegment seg);

    bool operator==(LineSegment seg);
};


#endif //NEPHESH_LINESEGMENT_H
