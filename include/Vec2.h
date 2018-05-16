#ifndef T1_VECTOR_H
#define T1_VECTOR_H


class Vec2 {
public:
    float x, y;
    Vec2();
    Vec2(float x, float y);
    float Module();
    Vec2 Normalize();
    float Distance(Vec2 target);
    float XAngle();
    float XAngleDeg();
    // Angle is in radians
    Vec2 Rotate(float angle);
    Vec2 RotateDeg(float angle);
    // Angle of target - this
    float DiferenceAngle(Vec2 target);

    // Operator overloads
    Vec2 operator+(Vec2 v2);
    Vec2 operator-(Vec2 v2);
    Vec2 operator*(float scalar);
    void operator+=(Vec2 v);
    void operator-=(Vec2 v);
    void operator*=(float scalar);
};


#endif //T1_VECTOR_H
