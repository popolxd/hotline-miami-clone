#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "player.h"

typedef enum {
    MELEE,
    RANGED,
    KILLED,
} ENEMY_EnemyType;

typedef struct {
    ENTITY_Entity entity;
    ENEMY_EnemyType type;
    float hp;
} ENEMY_Enemy;

void ENEMY_CreateMeleeEnemy(ENEMY_Enemy* enemy, Vector2 pos);
void ENEMY_UpdateMeleeEnemy(ENEMY_Enemy* enemy, PLAYER_Player* player, SHAPES_Polygon* levelPolygons, int levelPolygonsLen, Vector2 cameraOff);

void ENEMY_CreateRangedEnemy(ENEMY_Enemy* enemy, Vector2 pos);
void ENEMY_UpdateRangedEnemy(ENEMY_Enemy* enemy, PLAYER_Player* player, SHAPES_Polygon* levelPolygons, int levelPolygonsLen, PROJECTILES_Projectile* projectiles[2], int projectilesLen[2], Vector2 cameraOff);
#endif