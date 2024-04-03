#include "mymath.h"
#include "raylib.h"
#include <math.h>

Vector2 MYMATH_GetNormalizedVec(Vector2 vec)
{
    float divisor = sqrt(vec.x*vec.x + vec.y*vec.y);
    return (Vector2){vec.x / divisor, vec.y / divisor};
}

float MYMATH_GetDegRotationFromAnyVec(Vector2 vec)
{
    if (vec.x == 0) {
        if (vec.y > 0) return 90;
        return 270;
    }
    
    float rotation = atanf(vec.y/vec.x)/PI*180;
    if (vec.x < 0) rotation += 180;
    if (rotation < 0) rotation += 360;

    return rotation;
}