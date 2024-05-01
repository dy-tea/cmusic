#include <stdio.h>
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"

int get_minutes_round(float duration) {
  return ((int)duration / 60);
}

int get_seconds_round(float duration) {
  return ((int)duration % 60);
}

int main(int argc, char **argv) {
  const int screen_width = 800;
  const int screen_height = 800;

  InitWindow(screen_width, screen_height, "Music");
  
  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

  const int font_size_small = screen_width / 33;

  bool play = false;
  bool last_play = false;

  char song_name[255];
  int song_index = 0;
  if (argc > 1) {
    snprintf(song_name, 255, "%s", argv[song_index + 1]);
    printf("NOTE: Received \"%s\"\n", song_name);
  } else {
    printf("NOTE: No music file specified.\n");
  }

  // Music stream
  InitAudioDevice();
  Music music;
  float time_played = 0.0f;

  music = LoadMusicStream(song_name);
  PlayMusicStream(music);

  /*
  if (!music) {
    printf("ERROR: Failed to load \"%s\" to music stream.\n", song_name);
    return 1;
  }
  */

  while (!WindowShouldClose()) {
    /* Updating */
    UpdateMusicStream(music);

    // Play/Pause when space is pressed 
    if (IsKeyPressed(KEY_SPACE)) {
      play = !play;
      last_play = play;
      if (play) ResumeMusicStream(music);
      else PauseMusicStream(music);
    } else if (last_play != play) {
      // Play/Pause when play is changed 
      last_play = play;
      if (play) ResumeMusicStream(music);
      else PauseMusicStream(music);
    }

    // Update time played
    time_played = GetMusicTimePlayed(music) / GetMusicTimeLength(music);
    if (time_played > 1.0f) time_played = 1.0f; 
    char time_progress_str[6];
    snprintf(time_progress_str, 6, "%02d:%02d", get_minutes_round(GetMusicTimePlayed(music)), get_seconds_round(GetMusicTimePlayed(music)));
    char time_togo_str[6];
    float togo_duration = GetMusicTimeLength(music) - GetMusicTimePlayed(music);
    snprintf(time_togo_str, 6, "%02d:%02d", get_minutes_round(togo_duration), get_seconds_round(togo_duration));

    /* Drawing */
    BeginDrawing();

      ClearBackground(RAYWHITE);
      GuiToggle((Rectangle){screen_width / 2 - screen_width / 16, screen_height / 10 * 9 - font_size_small / 2, 100, font_size_small}, play ? "Play" : "Pause", &play);

      GuiProgressBar((Rectangle){screen_width / 2 - screen_width / 3, screen_height / 10 * 8, screen_width / 3 * 2, screen_height / 20}, time_progress_str, time_togo_str, &time_played, 0.0f, 1.0f);

    EndDrawing();
  }

  UnloadMusicStream(music);
  CloseAudioDevice();
  CloseWindow();

  return 0;
}
