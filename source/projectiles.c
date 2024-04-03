#include "projectiles.h"
#include "main.h"
#include <stdlib.h>

void PROJECTILES_AddBullet(PROJECTILES_Projectile** projectiles, int* projectilesLen, PROJECTILES_Projectile newProjectile)
{
    int foundSpot = 0;
    for (int i = 0; i < *projectilesLen; i++) {
        if ((*projectiles)[i].type == DEAD) {
            foundSpot = 1;
            (*projectiles)[i] = newProjectile;
            break;
        }
    }

    if (!foundSpot) {
        (*projectilesLen)++;
        (*projectiles) = (PROJECTILES_Projectile*)realloc(*projectiles, sizeof(PROJECTILES_Projectile)*(*projectilesLen));
        (*projectiles)[*projectilesLen - 1] = newProjectile;
    }
}