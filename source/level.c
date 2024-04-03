#include "level.h"
#include "player.h"
#include "main.h"
#include "collisions.h"
#include <raylib.h>
// #include <raymath.h>
#include <stdlib.h>
#include <stdio.h>

void LEVEL_InitLevel1(LEVEL_Level* level, PLAYER_Player* player)
{
    level->objectsLen = 1;
    level->objects = (SHAPES_Polygon*)malloc(sizeof(SHAPES_Polygon)*level->objectsLen);

    Vector2 polygon1Points[3] = {(Vector2){500, 500}, (Vector2){600, 600}, (Vector2){700, 500}};
    SHAPES_CreatePolygon(&level->objects[0], polygon1Points, 3);

    level->enemiesLen = 1;
    level->enemies = (ENEMY_Enemy*)malloc(sizeof(ENTITY_Entity)*level->enemiesLen);
    ENEMY_CreateRangedEnemy(&level->enemies[0], (Vector2){400, 400});

    PLAYER_InitPlayer(player, (Vector2){200, 200}, 50, 200);

    level->projectiles[FRIENDLY] = (PROJECTILES_Projectile*)malloc(sizeof(PROJECTILES_Projectile)*1);
    level->projectiles[FRIENDLY][0].type = DEAD;
    level->projectiles[ENEMY] = (PROJECTILES_Projectile*)malloc(sizeof(PROJECTILES_Projectile)*1);
    level->projectiles[ENEMY][0].type = DEAD;

    level->projectilesLen[FRIENDLY] = 1;
    level->projectilesLen[ENEMY] = 1;

    level->cameraOff = (Vector2){0, 0};
    level->cameraVel = (Vector2){50, 0};

}

void LEVEL_UpdateLevel1(LEVEL_Level* level, PLAYER_Player* player)
{
    PLAYER_UpdatePlayer(player, level->objects, level->objectsLen, level->projectiles, level->projectilesLen, &level->cameraOff);

    for (int i = 0; i < level->enemiesLen; i++) {
        switch (level->enemies[i].type) {
            case KILLED:
                break;
            case MELEE:
                ENEMY_UpdateMeleeEnemy(&level->enemies[i], player, level->objects, level->objectsLen, level->cameraOff);
                break;
            case RANGED:
                ENEMY_UpdateRangedEnemy(&level->enemies[i], player, level->objects, level->objectsLen, level->projectiles, level->projectilesLen, level->cameraOff);
                break;
        }
    }

    LEVEL_UpdateProjectiles(level, player);

    for (int i = 0; i < level->objectsLen; i++) {
        SHAPES_DrawPolygon(&level->objects[i], level->cameraOff);
    }
}

void LEVEL_UpdateProjectiles(LEVEL_Level* level, PLAYER_Player* player)
{
    for (int type = 0; type < 2; type++) {

        for (int i = 0; i < level->projectilesLen[type]; i++) {

            if (level->projectiles[type][i].type == DEAD) continue;

            Vector2 trueBulletVel = (Vector2){level->projectiles[type][i].vel.x*ELAPSED, level->projectiles[type][i].vel.y*ELAPSED};
            Vector2 temporary[2] = {
                level->projectiles[type][i].pos, (Vector2){level->projectiles[type][i].pos.x+trueBulletVel.x, level->projectiles[type][i].pos.y+trueBulletVel.y}
            };

            for (int j = 0; j < level->objectsLen; j++) { // wall collision
                if (COLLISIONS_IsLineSegmentPolygonIntersection(temporary, &level->objects[j])) {
                    level->projectiles[type][i].type = DEAD;
                    break;
                }
            }

            Vector2 truePolygonVel;
            if (type == FRIENDLY) { // jeden if ma nezabije hadam
                for (int j = 0; j < level->enemiesLen; j++) {
                    if (level->enemies[j].type == KILLED) continue;
                    
                    truePolygonVel = (Vector2){level->enemies[j].entity.vel.x*ELAPSED, level->enemies[j].entity.vel.y*ELAPSED};

                    if (COLLISIONS_IsMovingPointMovingCircleIntersection(level->projectiles[type][i].pos, trueBulletVel, &level->enemies[j].entity.circle, truePolygonVel)) {
                        level->projectiles[type][i].type = DEAD;
                        level->enemies[j].hp -= level->projectiles[type][i].dmg;
                        if (level->enemies[j].hp <= 0) level->enemies[j].type = KILLED;
                        break;
                    }
                }

            } else {
                truePolygonVel = (Vector2){player->entity.vel.x*ELAPSED, player->entity.vel.y*ELAPSED};
                if (COLLISIONS_IsMovingPointMovingCircleIntersection(level->projectiles[type][i].pos, trueBulletVel, &player->entity.circle, truePolygonVel)) {
                    level->projectiles[type][i].type = DEAD;

                    if (player->entity.timers[1] > 0) {
                        printf("player died\n");
                    }
                    else {
                        player->entity.timers[1] = 5; // cas, ktory player bude suseptible k smrti
                    }
                    break;
                }
            }

            switch (level->projectiles[type][i].type) {
                case BULLET:
                    level->projectiles[type][i].pos.x += level->projectiles[type][i].vel.x*ELAPSED;
                    level->projectiles[type][i].pos.y += level->projectiles[type][i].vel.y*ELAPSED;
                    
                    DrawCircle(level->projectiles[type][i].pos.x - level->cameraOff.x, level->projectiles[type][i].pos.y - level->cameraOff.y, 3, WHITE);
                    break;
                case DEAD:
                    break;
            }

        }
    }
}