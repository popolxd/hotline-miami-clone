#include "entity.h"
#include "shapes.h"
#include "collisions.h"

void ENTITY_CreateEntity(ENTITY_Entity* entity, Vector2 center, float radius, float maxVel)
{
    // SHAPES_CreatePolygon(&entity->polygon, points, pointsLen);
    entity->circle = (SHAPES_Circle){
        .center = center,
        .radius = radius
    };

    entity->rotation = 0;

    entity->vel = (Vector2){0, 0};
    entity->maxVel = maxVel;

    for (int i = 0; i < 5; i++) {
        entity->timers[i] = 0;
    }
    entity->timersLen = 0;
}

void ENTITY_ResolveCollisions(ENTITY_Entity* entity, SHAPES_Polygon* levelPolygons, int levelPolygonsLen)
{
    Vector2 normalEntityPersp;
    float sepVal;
    for (int i = 0; i < levelPolygonsLen; i++) {
        sepVal = COLLISIONS_GetCirclePolygonSeparationValue(&entity->circle, &levelPolygons[i], &normalEntityPersp);
        if (sepVal < 0) {
            entity->circle.center.x += normalEntityPersp.x*sepVal;
            entity->circle.center.y += normalEntityPersp.y*sepVal;
        }
    }
}