#ifndef T1_RECT_H
#define T1_RECT_H


#include "Vec2.h"

class Rect {
public:
    float x, y, h, w;
    Rect();
    Rect(float h, float w);
    Rect(float x, float y, float h, float w);

    Vec2 Center();
    float DistanceFrom(Rect target);
    bool Contains(Vec2 dot);

    //Overloaded operators
    Rect operator+(Vec2 v2);
    void operator+=(Vec2 v2);
};


#endif //T1_RECT_H
