#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "shapes.h"
#include "projectiles.h"
#include "entity.h"

typedef struct {
    ENTITY_Entity entity;
    int crouching;
} PLAYER_Player;

void PLAYER_InitPlayer(PLAYER_Player* player, Vector2 pos, float radius, float maxVel);
void PLAYER_UpdatePlayer(PLAYER_Player* player, SHAPES_Polygon* levelPolygons, int levelPolygonsLen, PROJECTILES_Projectile* projectiles[2], int projectilesLen[2], Vector2* cameraOff);

#endif