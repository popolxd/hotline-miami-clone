#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "raylib.h"
#include "shapes.h"

float COLLISIONS_GetPolygonsSeparationValue(SHAPES_Polygon* polygon1, SHAPES_Polygon* polygon2, Vector2* normalBody1Persp);

float COLLISIONS_GetCirclePolygonSeparationValue(SHAPES_Circle* circle, SHAPES_Polygon* polygon, Vector2* normalCirclePersp);

int COLLISIONS_IsMovingPointCircleIntersection(Vector2 point, Vector2 pointVel, SHAPES_Circle* circle);
int COLLISIONS_IsMovingPointMovingCircleIntersection(Vector2 point, Vector2 pointVel, SHAPES_Circle* circle, Vector2 circleVel);

int COLLISIONS_IsLineSegmentPolygonIntersection(Vector2 line[2], SHAPES_Polygon* polygon);
// int COLLISIONS_IsLineSegmentMovingPolygonIntersection(Vector2 line[2], SHAPES_Polygon* polygon, Vector2 polygonVel);

int COLLISIONS_IsLineSegmentsCollision(Vector2 line1[2], Vector2 line2[2]);
// int COLLISIONS_IsMovingLineSegmentLineSegmentCollision(Vector2 line1[2], Vector2 line2[2], Vector2 line2Vel);

#endif