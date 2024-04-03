#ifndef POLYGON_H
#define POLYGON_H

#include "raylib.h"

typedef struct {
    Vector2* points;
    Vector2* normals; // don't have to compute later
    int pointsLen;
} SHAPES_Polygon;

typedef struct {
    Vector2 center;
    float radius;
} SHAPES_Circle;

void SHAPES_CreatePolygon(SHAPES_Polygon* polygon, Vector2* points, int pointsLen);
void SHAPES_DrawPolygon(SHAPES_Polygon* polygon, Vector2 cameraOff);
Vector2 SHAPES_GetPolygonCenter(SHAPES_Polygon* polygon); // potentionally unused

void SHAPES_MovePolygon(SHAPES_Polygon* polygon, Vector2 vel); // potentionally unused

#endif