#include <raylib.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define FONT_SIZE 36
#define ATLAS_SIZE 1024

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

int main() {
  InitWindow(600, 400, "RayFT");
  SetTargetFPS(60);

  FT_Library ft;
  FT_Face face;
  const char *text = "Hello 世界 Привет";

  // Initialize library and font
  FT_Init_FreeType(&ft);
  FT_New_Face(ft, "assets/DMMono-Regular.ttf", 0, &face);
  FT_Select_Charmap(face, FT_ENCODING_UNICODE);
  FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);

  // Mapping codepoints in the string into glyph ids from the font
  char* ptr = (char*) text;
  while (*ptr != '\0') {
    int byte_count;
    int codepoint = GetCodepointNext(ptr, &byte_count);
    if (byte_count <= 0) {
      break;
    }
    FT_UInt glyph_index = FT_Get_Char_Index(face, codepoint);
    ptr += byte_count;
  }

  // Let's load the first glyph from the string
  int byte_count;
  int first_codepoint = GetCodepointNext(text, &byte_count);
  FT_UInt first_glyph_index = FT_Get_Char_Index(face, first_codepoint);
  FT_Load_Glyph(face, first_glyph_index, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT);
  // Now face.glyph contains the rasterized bitmap and its metrics
  FT_GlyphSlot glyph = face->glyph;
  FT_Bitmap* bitmap = &glyph->bitmap;
  // Dimensions of the bitmap
  int width = bitmap->width;
  int height = bitmap->rows;
  // Offset values for the glyph
  int offset_x = glyph->bitmap_left;
  int offset_y = glyph->bitmap_top;
  // How much to advance horizontally after drawing the glyph, in pixels
  float advance_x = glyph->advance.x / 64.0f;

  // Let's create an atlas to store the loaded glyphs
  Image image = GenImageColor(ATLAS_SIZE, ATLAS_SIZE, BLANK);
  Texture2D atlas = LoadTextureFromImage(image);
  UnloadImage(image);
  SetTextureFilter(atlas, TEXTURE_FILTER_POINT);

  // Write the glyph to the atlas
  Color* pixels = BitmapToRGBA(bitmap);
  // This rectangle should be dynamically generated to address the next
  // available position in the atlas
  Rectangle region = (Rectangle){ (float) 0, (float) 0, width, height };
  UpdateTextureRec(atlas, region, pixels);
  free(pixels);

  // This is the position we'll be drawing at
  int pen_x = 100;
  int pen_y = 100;

  float baseline_y = pen_y + face->size->metrics.ascender / 64.0f;

  float draw_x = pen_x + offset_x;
  float draw_y = baseline_y - offset_y;

  Rectangle destination_rect = (Rectangle) {
    draw_x,
    draw_y,
    width,
    height
  };

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    // Draw the actual glyph
    DrawTexturePro(
      atlas,
      region,
      destination_rect,
      (Vector2){ 0, 0 },
      0.0f,
      BLACK
    );

    EndDrawing();
  }

  CloseWindow();

  return 0;
}