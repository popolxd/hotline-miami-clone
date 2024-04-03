#ifndef LEVEL_H
#define LEVEL_H

#include "player.h"
#include "shapes.h"
#include "raylib.h"
#include "projectiles.h"
#include "entity.h"
#include "enemy.h"

typedef struct {
    SHAPES_Polygon* objects;
    int objectsLen;

    ENEMY_Enemy* enemies;
    int enemiesLen;

    PROJECTILES_Projectile* projectiles[2];
    int projectilesLen[2];

    Vector2 cameraOff;
    Vector2 cameraVel;

} LEVEL_Level;

void LEVEL_InitLevel1(LEVEL_Level* level, PLAYER_Player* player);
void LEVEL_UpdateLevel1(LEVEL_Level* level, PLAYER_Player* player);

void LEVEL_UpdateProjectiles(LEVEL_Level* level, PLAYER_Player* player);
#endif