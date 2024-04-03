#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "raylib.h"

#define FRIENDLY 0
#define ENEMY 1

typedef enum {
    BULLET,
    DEAD,
} PROJECTILES_Type;

typedef struct {
    PROJECTILES_Type type;

    Vector2 pos;
    Vector2 vel;
    float dmg;
} PROJECTILES_Projectile;

void PROJECTILES_AddBullet(PROJECTILES_Projectile** projectiles, int* projectilesLen, PROJECTILES_Projectile newProjectile);

#endif