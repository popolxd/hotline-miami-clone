#include "main.h"
#include "level.h"
#include "player.h"
#include "collisions.h"
#include "textures.h"
#include <raylib.h>

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
float ELAPSED;

int main(void)
{
    SCREEN_WIDTH = 1200;
    SCREEN_HEIGHT = 850;
    ELAPSED = 1;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "New Game!");

    LEVEL_Level level1;
    PLAYER_Player player;
    LEVEL_InitLevel1(&level1, &player);

    SetTargetFPS(60);

    TEXTURES_LoadAllTextures();

    // Main game loop
    while (!WindowShouldClose())
    {
        ELAPSED = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        LEVEL_UpdateLevel1(&level1, &player);

        DrawText(TextFormat("%.3f\n", ELAPSED*1000), 20, 20, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}