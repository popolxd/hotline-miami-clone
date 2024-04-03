#include "player.h"
#include "shapes.h"
#include "projectiles.h"
#include "collisions.h"
#include "entity.h"
#include "mymath.h"
#include "main.h"
#include "textures.h"
#include <math.h>

void PLAYER_InitPlayer(PLAYER_Player* player, Vector2 pos, float radius, float maxVel)
{
    ENTITY_CreateEntity(&player->entity, pos, radius, maxVel);
    player->crouching = 0;
    // timers:
    //     0: shooting
    //     1: if below zero, can die
}

void PLAYER_UpdatePlayer(PLAYER_Player* player, SHAPES_Polygon* levelPolygons, int levelPolygonsLen, PROJECTILES_Projectile* projectiles[2], int projectilesLen[2], Vector2* cameraOff)
{
    if (IsKeyDown(KEY_W)) player->entity.vel.y = -player->entity.maxVel;
    else if (IsKeyDown(KEY_S)) player->entity.vel.y = player->entity.maxVel;
    else player->entity.vel.y = 0;
    if (IsKeyDown(KEY_A)) player->entity.vel.x = -player->entity.maxVel;
    else if (IsKeyDown(KEY_D)) player->entity.vel.x = player->entity.maxVel;
    else player->entity.vel.x = 0;

    if (IsKeyDown(KEY_SPACE)) {
        player->entity.vel.x /= 2.2;
        player->entity.vel.y /= 2.2;
        player->crouching = 1;
    } else {
        player->crouching = 0;
    }

    // rotation calculation
    Vector2 mousePos = GetMousePosition();
    Vector2 playerToMouse = (Vector2){mousePos.x - (player->entity.circle.center.x - cameraOff->x), mousePos.y - (player->entity.circle.center.y - cameraOff->y)};
    player->entity.rotation = MYMATH_GetDegRotationFromAnyVec(playerToMouse);
    
    // timers ticking
    player->entity.timers[0] -= ELAPSED;
    player->entity.timers[1] -= ELAPSED;

    // bullet shooting
    if (player->entity.timers[0] <= 0 && IsMouseButtonDown(0)) {
        Vector2 normalizedBulletVel = MYMATH_GetNormalizedVec(playerToMouse);
        float bulletVel = 800;

        PROJECTILES_Projectile newBullet = (PROJECTILES_Projectile){
            .pos = player->entity.circle.center,
            .vel = (Vector2){normalizedBulletVel.x*bulletVel, normalizedBulletVel.y*bulletVel},
            .type = BULLET,
            .dmg = 10,
        };

        PROJECTILES_AddBullet(&projectiles[FRIENDLY], &projectilesLen[FRIENDLY], newBullet);
        player->entity.timers[0] = 0.25;
    }

    Vector2 trueVel = (Vector2){player->entity.vel.x*ELAPSED, player->entity.vel.y*ELAPSED};

    player->entity.circle.center.x += trueVel.x;
    player->entity.circle.center.y += trueVel.y;

    ENTITY_ResolveCollisions(&player->entity, levelPolygons, levelPolygonsLen);

    cameraOff->x = fmaxf(player->entity.circle.center.x - SCREEN_WIDTH*0.5, 0);
    cameraOff->y = fmaxf(player->entity.circle.center.y - SCREEN_HEIGHT*0.5, 0);

    // drawing
    DrawTexturePro(
        testTexture.texture, 
        (Rectangle){0, 0, testTexture.trueSize.x, testTexture.trueSize.y}, 
        (Rectangle){player->entity.circle.center.x - cameraOff->x, player->entity.circle.center.y - cameraOff->y, 2*player->entity.circle.radius, 2*player->entity.circle.radius}, 
        (Vector2){player->entity.circle.radius, player->entity.circle.radius}, 
        player->entity.rotation, 
        WHITE
    );
}