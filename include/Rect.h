#ifndef T1_RECT_H
#define T1_RECT_H


#include "Vec2.h"
#include <vector>

using namespace std;

class Rect {
public:
    float x, y, h, w;
    Rect();
    Rect(float h, float w);
    Rect(float x, float y, float h, float w);

    Vec2 Center();
    void PlaceCenterAt(Vec2 pos);
    float DistanceFrom(Rect target);
    bool Contains(Vec2 dot);
    vector<Vec2> GetCorners(float angle = 0, Vec2 rotationCenter = Vec2(0, 0));

    //Overloaded operators
    Rect operator+(Vec2 v2);
    void operator+=(Vec2 v2);
    Rect operator-(Vec2 v2);
    void operator-=(Vec2 v2);
    Rect& operator=(Vec2 vec);
};


#endif //T1_RECT_H
