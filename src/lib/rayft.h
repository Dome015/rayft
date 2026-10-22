#ifndef __RAYFT_H__
#define __RAYFT_H__

#include <raylib.h>
#include <ft2build.h>
#include FT_FREETYPE_H

// Initializes the RayFT library. Call this function before loading/drawing any font.
void RftInit(void);
// Coverts a FreeType bitmap into an array of colors to be printed on a texture.
Color* BitmapToRGBA(FT_Bitmap* bitmap);

#endif