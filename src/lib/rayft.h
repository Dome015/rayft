#ifndef __RAYFT_H__
#define __RAYFT_H__

#include <raylib.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define ATLAS_COUNT 8
#define CACHE_SIZE 128

typedef struct RftAtlas RftAtlas;
typedef struct RftCachedGlyph RftCachedGlyph;
typedef struct RftFont RftFont;

struct RftAtlas {
  Texture2D texture;
  Vector2 next_position;
  int row_height;
};

struct RftCachedGlyph {
  int index;
  int atlas_index;
  Vector2 atlas_position;
  int width;
  int height;
  int x_offset;
  int y_offset;
  float x_advance;
  RftCachedGlyph* next;
};

struct RftFont {
  FT_Face face;
  RftAtlas atlases[ATLAS_COUNT];
  int used_atlases;
  RftCachedGlyph* cache[CACHE_SIZE];
};

// Initializes the RayFT library. Call this function before loading/drawing any font.
void RftInit(void);
// Coverts a FreeType bitmap into an array of colors to be
// printed on a texture.
Color* RftBitmapToRGBA(FT_Bitmap* bitmap);
// Create an empty atlas
RftAtlas RftCreateAtlas(void);
// Destroy the atlas (unload its texture)
void RftDestroyAtlas(RftAtlas atlas);
// Checks whether there is an available position in the atlas for the given dimensions
bool RftAtlasHasSpace(RftAtlas atlas, Vector2 dim);
// Draws the given pixels at the next available position in the atlas (if present),
// following the provided dimensions. Returns the position at which the glyph was placed,
// which is placed at (-1, -1) if the placement was unsuccessful
Vector2 RftAddToAtlas(RftAtlas* atlas, Color* pixels, Vector2 dim);
// Loads the font at the provided path and sets its initial size
RftFont RftLoadFont(char* path, int size);
// Removes the font's resources from memory
void RftUnloadFont(RftFont font);
// Returns the hash value for x
int RftHash(int x);
// Returns the font glyph at the provided index, loading it and adding it to the cache
// if it wasn't already there.
RftCachedGlyph* RftGetGlyph(RftFont* font, int index);
// Returns the cached info for the glyph at the given index if present, otherwise NULL.
RftCachedGlyph* RftFindGlyph(RftFont font, int index);
// Loads and caches the glyph at the given index, returning a pointer to it.
RftCachedGlyph* RftLoadGlyph(RftFont* font, int index);
// Draws the glyph at the given index at the given position. Returns the position at which the
// next glyph should be drawn on the same line.
Vector2 RftDrawGlyph(RftFont* font, int index, Vector2 position, Color color);
// Draws the text at the given position using the specified font and color.
void RftDrawText(RftFont* font, const char* text, Vector2 position, Color color);

#endif