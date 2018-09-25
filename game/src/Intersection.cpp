//
// Created by ftfnunes on 07/07/18.
//

#include "Intersection.h"

Intersection::Intersection(LineSegment colliderLine, LineSegment collidableLine, Vec2 intersectionPoint) : colliderLine(colliderLine),
                                                                                                      collidableLine(collidableLine),
                                                                                                      intersectionPoint(intersectionPoint) {
}
