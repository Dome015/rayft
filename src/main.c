#include <raylib.h>
#include "lib/rayft.h"

#define FONT_SIZE 14

int main() {
  InitWindow(600, 400, "RayFT");
  SetTargetFPS(60);
  const char *text = "Hello, world!";

  // Initialize library and font
  RftInit();
  RftFont font = RftLoadFont("assets/DMMono-Regular.ttf", FONT_SIZE);
  
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    RftDrawText(&font, text, (Vector2){ 100, 100 }, BLACK);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}