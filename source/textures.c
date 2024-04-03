#include "textures.h"
#include "mymath.h"
#include <raylib.h>

TEXTURES_TextureInfo testTexture;

void TEXTURES_LoadAllTextures()
{
    testTexture.texture = LoadTexture("resources/test.png");
    testTexture.trueSize = (IntVec2){900, 600};
}