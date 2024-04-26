#include <stdio.h>
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"

int main() {
  const int screen_width = 800;
  const int screen_height = 800;

  InitWindow(screen_width, screen_height, "Music");
  
  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

  const int font_size_small = 24;

  bool play = false;

  // Progress in song FIXME
  float song_progress = 10.0f;
  float song_length = 100.0f; 
  float song_percentage = song_progress / song_length * 100.0f;
  
  // Float progress amount to string FIXME
  int song_progress_len = snprintf(NULL, 0, "%f", song_progress);
  char *song_progress_text = malloc(song_progress_len + 1);
  snprintf(song_progress_text, song_progress_len + 1, "%.2f", song_progress);

  // Float to go amount to string FIXME
  int song_togo_len = snprintf(NULL, 0, "%f", song_length - song_progress);
  char *song_togo_text = malloc(song_togo_len + 1);
  snprintf(song_togo_text, song_togo_len + 1, "%.2f", song_length - song_progress);

  while (!WindowShouldClose()) {

    BeginDrawing();

      ClearBackground(RAYWHITE);
      GuiToggle((Rectangle){screen_width / 2 - screen_width / 16, screen_height / 10 * 9 - font_size_small / 2, 100, font_size_small}, "Play/Pause", &play);

      GuiProgressBar((Rectangle){screen_width / 2 - screen_width / 3, screen_height / 10 * 8, screen_width / 3 * 2, screen_height / 20}, song_progress_text, song_togo_text, &song_percentage, 0.0f, 100.0f);


    EndDrawing();
  }

  // FIXME
  free(song_progress_text);
  free(song_togo_text);

  CloseWindow();

  return 0;
}
