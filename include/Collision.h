//
// Created by ftfnunes on 17/04/18.
//

#ifndef T1_COLLISION_H
#define T1_COLLISION_H

#include "Rect.h"
#include "Vec2.h"

#include <algorithm>
#include <cmath>

class Collision {

public:
    // Observação: IsColliding espera ângulos em radianos!
    // Para usar graus, forneça a sua própria implementação de Rotate,
    // ou transforme os ângulos no corpo de IsColliding.
    static inline bool IsColliding(Rect& a, Rect& b, float angleOfA, float angleOfB) {
        auto aCorners = a.GetCorners(angleOfA);
        auto bCorners = b.GetCorners(angleOfB);
        vector<Vec2> A = { aCorners[3],
                     aCorners[2],
                     aCorners[1],
                     aCorners[0]
        };
        vector<Vec2> B = { aCorners[3],
                           aCorners[2],
                           aCorners[1],
                           aCorners[0]
        };

        Vec2 axes[] = { (A[0] - A[1]).Normalize(), (A[1] - A[2]).Normalize(), (B[0] - B[1]).Normalize(), (B[1] - B[2]).Normalize() };

        for (auto& axis : axes) {
            float P[4];

            for (int i = 0; i < 4; ++i) P[i] = Dot(A[i], axis);

            float minA = *std::min_element(P, P + 4);
            float maxA = *std::max_element(P, P + 4);

            for (int i = 0; i < 4; ++i) P[i] = Dot(B[i], axis);

            float minB = *std::min_element(P, P + 4);
            float maxB = *std::max_element(P, P + 4);

            if (maxA < minB || minA > maxB)
                return false;
        }

        return true;
    }

private:

    static inline float Dot(const Vec2& a, const Vec2& b) {
        return a.x * b.x + a.y * b.y;
    }
};


#endif //T1_COLLISION_H
