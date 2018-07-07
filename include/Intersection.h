//
// Created by ftfnunes on 07/07/18.
//

#ifndef NEPHESH_LINEINTERSECTION_H
#define NEPHESH_LINEINTERSECTION_H


#include "LineSegment.h"

class Intersection {
public:
    LineSegment colliderLine;
    LineSegment collidableLine;
    Vec2 intersectionPoint;

    Intersection(LineSegment colliderLine, LineSegment collidableLine, Vec2 intersectionPoint);
};


#endif //NEPHESH_LINEINTERSECTION_H
