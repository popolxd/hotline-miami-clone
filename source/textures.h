#ifndef TEXTURES_H
#define TEXTURES_H

#include <raylib.h>
#include "mymath.h"

typedef struct {
    Texture2D texture;
    IntVec2 trueSize;
} TEXTURES_TextureInfo;

extern TEXTURES_TextureInfo testTexture;

void TEXTURES_LoadAllTextures();

#endif