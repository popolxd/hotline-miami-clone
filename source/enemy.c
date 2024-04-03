#include "enemy.h"
#include "entity.h"
#include "player.h"
#include "shapes.h"
#include "projectiles.h"
#include "mymath.h"
#include "main.h"
#include "textures.h"
#include <math.h>

void ENEMY_CreateMeleeEnemy(ENEMY_Enemy* enemy, Vector2 pos)
{
    enemy->type = MELEE;
    enemy->hp = 100;

    ENTITY_CreateEntity(&enemy->entity, pos, 50, 150);
}

void ENEMY_CreateRangedEnemy(ENEMY_Enemy* enemy, Vector2 pos)
{
    enemy->type = RANGED;
    enemy->hp = 80;

    ENTITY_CreateEntity(&enemy->entity, pos, 40, 100);
}

void ENEMY_UpdateMeleeEnemy(ENEMY_Enemy* enemy, PLAYER_Player* player, SHAPES_Polygon* levelPolygons, int levelPolygonsLen, Vector2 cameraOff)
{

    Vector2 enemyToPlayer = (Vector2){player->entity.circle.center.x - enemy->entity.circle.center.x, player->entity.circle.center.y - enemy->entity.circle.center.y};
    enemy->entity.rotation = MYMATH_GetDegRotationFromAnyVec(enemyToPlayer);

    enemy->entity.vel = MYMATH_GetNormalizedVec(enemyToPlayer);
    enemy->entity.vel.x *= enemy->entity.maxVel;
    enemy->entity.vel.y *= enemy->entity.maxVel;

    Vector2 trueVel = (Vector2){enemy->entity.vel.x*ELAPSED, enemy->entity.vel.y*ELAPSED};

    enemy->entity.circle.center.x += trueVel.x;
    enemy->entity.circle.center.y += trueVel.y;

    ENTITY_ResolveCollisions(&enemy->entity, levelPolygons, levelPolygonsLen);

    DrawTexturePro(
        testTexture.texture, 
        (Rectangle){0, 0, testTexture.trueSize.x, testTexture.trueSize.y}, 
        (Rectangle){enemy->entity.circle.center.x - cameraOff.x, enemy->entity.circle.center.y - cameraOff.y, 2*enemy->entity.circle.radius, 2*enemy->entity.circle.radius}, 
        (Vector2){enemy->entity.circle.radius, enemy->entity.circle.radius}, 
        enemy->entity.rotation, 
        WHITE
    );
}

void ENEMY_UpdateRangedEnemy(ENEMY_Enemy* enemy, PLAYER_Player* player, SHAPES_Polygon* levelPolygons, int levelPolygonsLen, PROJECTILES_Projectile* projectiles[2], int projectilesLen[2], Vector2 cameraOff)
{
    Vector2 enemyToPlayer = (Vector2){player->entity.circle.center.x - enemy->entity.circle.center.x, player->entity.circle.center.y - enemy->entity.circle.center.y};
    enemy->entity.rotation = MYMATH_GetDegRotationFromAnyVec(enemyToPlayer);

    if (sqrt(enemyToPlayer.x*enemyToPlayer.x + enemyToPlayer.y*enemyToPlayer.y) > 250) {
        enemy->entity.vel = MYMATH_GetNormalizedVec(enemyToPlayer);
        enemy->entity.vel.x *= enemy->entity.maxVel;
        enemy->entity.vel.y *= enemy->entity.maxVel;
    } else {
        enemy->entity.vel = (Vector2){0, 0};
    }

    enemy->entity.timers[0] -= ELAPSED;
    if (enemy->entity.timers[0] <= 0) {
        Vector2 normalizedBulletVel = MYMATH_GetNormalizedVec(enemyToPlayer);
        float bulletVel = 800;

        PROJECTILES_Projectile newBullet = (PROJECTILES_Projectile){
            .pos = enemy->entity.circle.center,
            .vel = (Vector2){normalizedBulletVel.x*bulletVel, normalizedBulletVel.y*bulletVel},
            .type = BULLET,
            .dmg = 10,
        };

        PROJECTILES_AddBullet(&projectiles[ENEMY], &projectilesLen[ENEMY], newBullet);
        enemy->entity.timers[0] = 1;
    }

    Vector2 trueVel = (Vector2){enemy->entity.vel.x*ELAPSED, enemy->entity.vel.y*ELAPSED};

    enemy->entity.circle.center.x += trueVel.x;
    enemy->entity.circle.center.y += trueVel.y;

    ENTITY_ResolveCollisions(&enemy->entity, levelPolygons, levelPolygonsLen);

    DrawTexturePro(
        testTexture.texture, 
        (Rectangle){0, 0, testTexture.trueSize.x, testTexture.trueSize.y}, 
        (Rectangle){enemy->entity.circle.center.x - cameraOff.x, enemy->entity.circle.center.y - cameraOff.y, 2*enemy->entity.circle.radius, 2*enemy->entity.circle.radius}, 
        (Vector2){enemy->entity.circle.radius, enemy->entity.circle.radius}, 
        enemy->entity.rotation, 
        WHITE
    );
}