#include "rayft.h"

#define ATLAS_COUNT 8
#define CACHE_SIZE 128
#define ATLAS_SIZE 1024

static FT_Library ft;

typedef struct RftAtlas {
  Texture2D texture;
  Vector2 next_position;
} RftAtlas;

typedef struct RftCachedGlyph {
  int index;
  int atlas_index;
  Rectangle atlas_rect;
  int x_offset;
  int y_offset;
  float x_advance;
  RftCachedGlyph* next;
} RftCachedGlyph;

typedef struct RftFont {
  FT_Face face;
  RftAtlas atlases[ATLAS_COUNT];
  RftCachedGlyph* cache[CACHE_SIZE]
} RftFont;

// Initializes the RayFT library. Call this function before loading/drawing any font.
void RftInit(void) {
  FT_Init_FreeType(&ft);
}

// Coverts a FreeType bitmap into an array of colors to be
// printed on a texture.
Color* BitmapToRGBA(FT_Bitmap* bitmap) {
  int w = bitmap->width;
  int h = bitmap->rows;
  Color* pixels = (Color*) malloc(w * h * sizeof(Color));
  int row_offset = 0;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      pixels[y * w + x] = (Color){ .r = 255, .g = 255, .b = 255, .a = bitmap->buffer[row_offset + x]};
    }
    row_offset += bitmap->pitch;
  }
  return pixels;
}

// Create an empty atlas
RftAtlas RftCreateAtlas(void) {
  Image image = GenImageColor(ATLAS_SIZE, ATLAS_SIZE, BLANK);
  RftAtlas atlas = (RftAtlas) {
    .texture = LoadTextureFromImage(image),
    .next_position = (Vector2){ 0, 0 }
  };
  UnloadImage(image);
  return atlas;
}

// Destroy the atlas (unload its texture)
void RftDestroyAtlas(RftAtlas atlas) {
  UnloadTexture(atlas.texture);
}

// Loads the font at the provided path and sets its initial size
RftFont RftLoadFont(char* path, int size) {
  RftFont font;
  FT_Face face;
  FT_New_Face(ft, "assets/DMMono-Regular.ttf", 0, &face);
  FT_Select_Charmap(face, FT_ENCODING_UNICODE);
  FT_Set_Pixel_Sizes(face, 0, size);
  font.face = face;
  for (int i = 0; i < CACHE_SIZE; i++) {
    font.cache[i] = NULL;
  }
  for (int i = 0; i < ATLAS_COUNT; i++) {
    font.atlases[i] = RftCreateAtlas();
  }
}

// Removes the font's resources from memory
void RftUnloadFont(RftFont font) {
  for (int i = 0; i < CACHE_SIZE; i++) {
    RftCachedGlyph* next_glyph = font.cache[i];
    RftCachedGlyph* glyph = NULL;
    while (next_glyph != NULL) {
      glyph = next_glyph;
      next_glyph = glyph->next;
      free(glyph);
    }
  }
  for (int i = 0; i < ATLAS_COUNT; i++) {
    RftDestroyAtlas(font.atlases[i]);
  }
  FT_Done_Face(font.face);
}

// Returns the hash value for x
int RftHash(int x) {
    x ^= x >> 16;
    x *= 0x85ebca6bU;
    x ^= x >> 13;
    x *= 0xc2b2ae35U;
    x ^= x >> 16;
    return x;
}

// Returns the cached info for the glyph at the given index if present, otherwise NULL.
RftCachedGlyph* RftFindCachedGlyph(RftFont font, int index) {
  for (RftCachedGlyph* g = font.cache[RftHash(index) % CACHE_SIZE]; g != NULL; g = g->next) {
    if (g->index == index) {
      return g;
    }
  }
  return NULL;
}

// Loads and caches the glyph at the given index, returning a pointer to it.
RftCachedGlyph* RftLoadGlyph(RftFont* font, int index) {
  RftCachedGlyph* glyph = (RftCachedGlyph*) malloc(sizeof(RftCachedGlyph));
  glyph->index = index;
}

void RftDrawText(RftFont* font, const char* text, Vector2 position) {
  // TODO
  /*char* ptr = (char*) text;
  while (*ptr != '\0') {
    int byte_count;
    int codepoint = GetCodepointNext(ptr, &byte_count);
    FT_UInt glyph_index = FT_Get_Char_Index(font.face, codepoint);
    ptr += byte_count;
    // Check if it's a newline; in this case, we need to add the line break
    if (codepoint == '\r' && )
    if (codepoint == '\n' || codepoint == '\r')
  }*/
}