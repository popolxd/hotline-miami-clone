#include "collisions.h"
#include "raylib.h"
#include "shapes.h"
#include "mymath.h"
#include "main.h"
#include <math.h>
#include <stdio.h>

float COLLISIONS_GetPolygonsSeparationValue(SHAPES_Polygon* polygon1, SHAPES_Polygon* polygon2, Vector2* normalBody1Persp)
{
    float maxSep = -INFINITY;
    float minSepForThisEdge;
    Vector2 pointsDifference;

    // prvy polygon
    for (int i = 0; i < polygon1->pointsLen; i++) {
        minSepForThisEdge = INFINITY;
        
        for (int j = 0; j < polygon2->pointsLen; j++) {
            pointsDifference = (Vector2){polygon2->points[j].x - polygon1->points[i].x, polygon2->points[j].y - polygon1->points[i].y};
            float projection = define_GetDotProd(pointsDifference, polygon1->normals[i]);

            if (projection < minSepForThisEdge) {
                minSepForThisEdge = projection;
            }
        }

        if (minSepForThisEdge > maxSep) {
            maxSep = minSepForThisEdge;
            *normalBody1Persp = polygon1->normals[i];
        }
    }

    // druhy polygon
    for (int i = 0; i < polygon2->pointsLen - 1; i++) {
        minSepForThisEdge = INFINITY;
        
        for (int j = 0; j < polygon1->pointsLen; j++) {
            pointsDifference = (Vector2){polygon1->points[j].x - polygon2->points[i].x, polygon1->points[j].y - polygon2->points[i].y};
            float projection = define_GetDotProd(pointsDifference, polygon2->normals[i]);

            if (projection < minSepForThisEdge) {
                minSepForThisEdge = projection;
            }
        }

        if (minSepForThisEdge > maxSep) {
            maxSep = minSepForThisEdge;
            normalBody1Persp->x = -polygon2->normals[i].x;
            normalBody1Persp->y = -polygon2->normals[i].y;
        }
    }

    return maxSep;
}

float COLLISIONS_GetCirclePolygonSeparationValue(SHAPES_Circle* circle, SHAPES_Polygon* polygon, Vector2* normalCirclePersp)
{
    float maxSep = -INFINITY;
    Vector2 pointsDifference;

    float projection;

    Vector2 closestPoint;
    float minDistSq = INFINITY;

    // polygon
    for (int i = 0; i < polygon->pointsLen; i++) {
        pointsDifference = (Vector2){circle->center.x - polygon->points[i].x, circle->center.y - polygon->points[i].y};

        float distSq = pointsDifference.x*pointsDifference.x + pointsDifference.y*pointsDifference.y;
        if (distSq < minDistSq) {
            minDistSq = distSq;
            closestPoint = polygon->points[i];
        }

        projection = define_GetDotProd(pointsDifference, polygon->normals[i]) - circle->radius;

        if (projection > maxSep) {
            maxSep = projection;
            normalCirclePersp->x = -polygon->normals[i].x;
            normalCirclePersp->y = -polygon->normals[i].y;
        }
    }

    Vector2 normal = MYMATH_GetNormalizedVec((Vector2){closestPoint.x - circle->center.x, closestPoint.y - circle->center.y});
    float minProj = INFINITY;
    float maxProj = -INFINITY;

    for (int i = 0; i < polygon->pointsLen; i++) {
        Vector2 circleCenterToPoint = (Vector2){polygon->points[i].x - circle->center.x, polygon->points[i].y - circle->center.y};
        projection = define_GetDotProd(circleCenterToPoint, normal);

        if (projection < minProj) minProj = projection;
        if (projection > maxProj) maxProj = projection;
    }

    float dist1 = fabsf(minProj - circle->radius);
    float dist2 = fabsf(maxProj + circle->radius);

    if (dist1 < dist2) {
        if (minProj < circle->radius) projection = -dist1;
        else projection = dist1;
    } else {
        if (maxProj > -circle->radius) projection = -dist2;
        else projection = dist2;
    }

    if (projection >= maxSep) {
        maxSep = projection;
        *normalCirclePersp = normal;
    }

    return maxSep;
}

int COLLISIONS_IsMovingPointCircleIntersection(Vector2 point, Vector2 pointVel, SHAPES_Circle* circle)
{
    float a = pointVel.x*pointVel.x + pointVel.y*pointVel.y;
    float b = 2*(pointVel.x*(point.x - circle->center.x) + pointVel.y*(point.y - circle->center.y));
    float c = (point.x - circle->center.x)*(point.x - circle->center.x) + (point.y - circle->center.y)*(point.y - circle->center.y) - circle->radius*circle->radius;

    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) return 0;

    float t;
    if (discriminant == 0) {
        t = -b/(2*a);
        if (t >= 0 && t <= 1) return 1;
        return 0;
    }

    t = (-b - sqrt(discriminant)) / (2*a);
    if (t >= 0 && t <= 1) return 1;
    t = (-b + sqrt(discriminant)) / (2*a);
    if (t >= 0 && t <= 1) return 1;
    return 0;
}

int COLLISIONS_IsMovingPointMovingCircleIntersection(Vector2 point, Vector2 pointVel, SHAPES_Circle* circle, Vector2 circleVel)
{
    float a = (pointVel.x - circleVel.x)*(pointVel.x - circleVel.x) + (pointVel.y - circleVel.y)*(pointVel.y - circleVel.y);
    float b = 2*((pointVel.x - circleVel.x)*(point.x - circle->center.x) + (pointVel.y - circleVel.y)*(point.y - circle->center.y));
    float c = (point.x - circle->center.x)*(point.x - circle->center.x) + (point.y - circle->center.y)*(point.y - circle->center.y) - circle->radius*circle->radius;

    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) return 0;

    float t;
    if (discriminant == 0) {
        t = -b/(2*a);
        if (t >= 0 && t <= 1) return 1;
        return 0;
    }

    t = (-b - sqrt(discriminant)) / (2*a);
    if (t >= 0 && t <= 1) return 1;
    t = (-b + sqrt(discriminant)) / (2*a);
    if (t >= 0 && t <= 1) return 1;
    return 0;
}

int COLLISIONS_IsLineSegmentPolygonIntersection(Vector2 line[2], SHAPES_Polygon* polygon)
{
    Vector2 currentPolygonLine[2];
    for (int i = 0; i < polygon->pointsLen-1; i++) {
        currentPolygonLine[0] = polygon->points[i];
        currentPolygonLine[1] = polygon->points[i+1];
        if (COLLISIONS_IsLineSegmentsCollision(line, currentPolygonLine)) {
            return 1;
        }
    }

    currentPolygonLine[0] = polygon->points[polygon->pointsLen-1];
    currentPolygonLine[1] = polygon->points[0];
    if (COLLISIONS_IsLineSegmentsCollision(line, currentPolygonLine)) {
        return 1;
    }

    return 0;
}

// int COLLISIONS_IsLineSegmentMovingPolygonIntersection(Vector2 line[2], SHAPES_Polygon* polygon, Vector2 polygonVel)
// {
//     Vector2 currentPolygonLine[2];

//     for (int i = 0; i < polygon->pointsLen-1; i++) {
//         currentPolygonLine[0] = polygon->points[i];
//         currentPolygonLine[1] = polygon->points[i+1];
//         if (COLLISIONS_IsMovingLineSegmentLineSegmentCollision(line, currentPolygonLine, polygonVel)) {
//             return 1;
//         }
//     }

//     currentPolygonLine[0] = polygon->points[polygon->pointsLen-1];
//     currentPolygonLine[1] = polygon->points[0];
//     if (COLLISIONS_IsMovingLineSegmentLineSegmentCollision(line, currentPolygonLine, polygonVel)) {
//         return 1;
//     }

//     return 0;
// }

int COLLISIONS_IsLineSegmentsCollision(Vector2 line1[2], Vector2 line2[2])
{
    float t1 = ((line2[0].y - line1[0].y)*(line2[1].x - line2[0].x) + (line2[1].y - line2[0].y)*(line1[0].x - line2[0].x)) 
    / ((line1[1].y - line1[0].y)*(line2[1].x - line2[0].x) - (line2[1].y - line2[0].y)*(line1[1].x - line1[0].x));

    if (t1 >= 0 && t1 <= 1) { // moze byt kolizia
        float t2;

        if (line2[1].x - line2[0].x != 0) {
            t2 = ((line1[0].x - line2[0].x) + (line1[1].x - line1[0].x)*t1) / (line2[1].x - line2[0].x);
            if (t2 >= 0 && t2 <= 1) return 1;
            return 0;
        } else {
            t2 = ((line1[0].y - line2[0].y) + (line1[1].y - line1[0].y)*t1) / (line2[1].y - line2[0].y);
            if (t2 >= 0 && t2 <= 1) return 1;
            return 0;
        }
    }
    return 0;
}

// int COLLISIONS_IsMovingLineSegmentLineSegmentCollision(Vector2 line1[2], Vector2 line2[2], Vector2 line2Vel)
// {
//     float t1 = ((line2[0].y - line1[0].y)*(line2[1].x - line2[0].x) + (line2[1].y - line2[0].y)*(line1[0].x - line2[0].x)) 
//     / ((line1[1].y - line1[0].y - line2Vel.y)*(line2[1].x - line2[0].x) - (line2[1].y - line2[0].y)*(line1[1].x - line1[0].x - line2Vel.x));

//     if (t1 >= 0 && t1 <= 1) { // moze byt kolizia
//         float t2;

//         if (line2[1].x - line2[0].x != 0) {
//             t2 = ((line1[0].x - line2[0].x) + (line1[1].x - line1[0].x - line2Vel.x)*t1) / (line2[1].x - line2[0].x);
//             if (t2 >= 0 && t2 <= 1) return 1;
//             return 0;
//         } else {
//             t2 = ((line1[0].y - line2[0].y) + (line1[1].y - line1[0].y - line2Vel.y)*t1) / (line2[1].y - line2[0].y);
//             if (t2 >= 0 && t2 <= 1) return 1;
//             return 0;
//         }
//     }
//     return 0;
// }