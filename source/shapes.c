#include "shapes.h"
#include "mymath.h"
#include <stdlib.h>

void SHAPES_CreatePolygon(SHAPES_Polygon* polygon, Vector2* points, int pointsLen)
{
    polygon->pointsLen = pointsLen;
    polygon->points = (Vector2*)malloc(sizeof(Vector2)*pointsLen);
    polygon->normals = (Vector2*)malloc(sizeof(Vector2)*pointsLen);

    for (int i = 0; i < pointsLen; i++) {
        polygon->points[i] = points[i];
    }

    // normals
    Vector2 edge;
    for (int i = 0; i < pointsLen-1; i++) {
        edge = (Vector2){points[i+1].x - points[i].x, points[i+1].y - points[i].y};
        polygon->normals[i] = MYMATH_GetNormalizedVec((Vector2){-edge.y, edge.x});
    }
    edge = (Vector2){points[0].x - points[pointsLen - 1].x, points[0].y - points[pointsLen - 1].y};
    polygon->normals[pointsLen - 1] = MYMATH_GetNormalizedVec((Vector2){-edge.y, edge.x});
}

Vector2 SHAPES_GetPolygonCenter(SHAPES_Polygon* polygon)
{
    Vector2 sum = (Vector2){0, 0};
    for (int i = 0; i < polygon->pointsLen; i++) {
        sum.x += polygon->points[i].x;
        sum.y += polygon->points[i].y;
    }

    return (Vector2){sum.x / polygon->pointsLen, sum.y / polygon->pointsLen};
}

void SHAPES_DrawPolygon(SHAPES_Polygon* polygon, Vector2 cameraOff)
{
    for (int i = 0; i < polygon->pointsLen - 1; i++) {
        DrawLine(polygon->points[i+1].x - cameraOff.x, polygon->points[i+1].y - cameraOff.y, polygon->points[i].x - cameraOff.x, polygon->points[i].y - cameraOff.y, WHITE);
    }
    DrawLine(polygon->points[0].x - cameraOff.x, polygon->points[0].y - cameraOff.y, polygon->points[polygon->pointsLen-1].x - cameraOff.x, polygon->points[polygon->pointsLen-1].y - cameraOff.y, WHITE);
}

void SHAPES_MovePolygon(SHAPES_Polygon* polygon, Vector2 vel)
{
    for (int i = 0; i < polygon->pointsLen; i++) {
        polygon->points[i].x += vel.x;
        polygon->points[i].y += vel.y;
    }
}