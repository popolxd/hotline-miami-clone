#ifndef MYMATH_H
#define MYMATH_H

#include "raylib.h"

#define define_GetDotProd(vec1, vec2) (vec1.x*vec2.x + vec1.y*vec2.y)

typedef struct {
    int x, y;
} IntVec2; // toto neviem ci tu ostane

Vector2 MYMATH_GetNormalizedVec(Vector2 vec);
float MYMATH_GetDegRotationFromAnyVec(Vector2 vec);

#endif