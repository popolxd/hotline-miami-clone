#ifndef ENTITY_H
#define ENTITY_H

#include "shapes.h"
#include <raylib.h>

typedef struct {
    SHAPES_Circle circle;

    float rotation;

    Vector2 vel;
    float maxVel;

    float timers[5];
    float timersLen;
} ENTITY_Entity;

void ENTITY_CreateEntity(ENTITY_Entity* entity, Vector2 center, float radius, float maxVel);
void ENTITY_ResolveCollisions(ENTITY_Entity* entity, SHAPES_Polygon* levelPolygons, int levelPolygonsLen);

#endif