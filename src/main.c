#include <stdio.h>
#include <unistd.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

// raygui embedded styles
#define MAX_GUI_STYLES_AVAILABLE 12
#include "style_jungle.h"
//#include "style_candy.h"
#include "style_lavanda.h"
#include "style_cyber.h"
#include "style_terminal.h"
//#include "style_ashes.h"
#include "style_bluish.h"
#include "style_dark.h"
//#include "style_cherry.h"
//#include "style_sunny.h"
//#include "style_enefete.h"

int get_minutes_round(float duration) {
  return ((int)duration / 60);
}

int get_seconds_round(float duration) {
  return ((int)duration % 60);
}

bool collide_point_rect(Vector2 p, Rectangle r) {
  return r.x + r.width >= p.x && r.x <= p.x && r.y + r.height >= p.y && r.y <= p.y;
}

char* basename(char* path) {
  int len = strlen(path);

  for (int i = len - 1; i > 0; i--)
    if (path[i] == '/') {
      path = path + i + 1;
      break;
    }

  return path;
}

char *get_image_file(const char *path) {
  char command[1024];
  unsigned char data[51200];

  const char* out_path = "/tmp/";

  const char* filename = basename(path);
  printf("BASENAME: %s\n", filename);

  char filename_cpy[strlen(filename)]; 
  memcpy(filename_cpy, filename, strlen(filename));
  char *no_ext = strtok(filename_cpy, ".");
  
  sprintf(command, "ffmpeg -n -i \"%s\" -filter:v scale=512:-1 -update true \"%s%s.png\"", path, out_path, no_ext);
  printf("NOTE: Running command \"%s\"\n", command);
  system(command);

  char *file_path = malloc(255);
  sprintf(file_path, "%s%s.png", out_path, no_ext);

  return file_path;
}

char* file_dialog() {
  char command[256];
  snprintf(command, sizeof(command), "zenity --file-selection --title='Open Audio File' --file-filter='Music Files | *.mp3 *.wav *.ogg *.flac'");

  FILE *fp = popen(command, "r");
  if (fp == NULL) {
    printf("ERROR: Failed to open file dialog process");
    return "";
  }

  static char path[1024];
  if (fgets(path, sizeof(path), fp) != NULL) {
    path[strcspn(path, "\n")] = 0;
    printf("NOTE: Selected file: %s\n", path);
  } else {
    printf("NOTE: No file selected");
  }

  pclose(fp);

  return path;
}

int main(int argc, char **argv) {
  const int screen_width = 800;
  const int screen_height = 800;

  InitWindow(screen_width, screen_height, "Music");
  
  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

  const int font_size_small = screen_width / 33; // Tenth of third
  int style_active = 0;
  int prev_style_active = 0;

  bool play = true;
  bool last_play = true;
  bool music_loaded = false;

  char song_name[1024];
  int song_index = 0;
  if (argc > 1) {
    snprintf(song_name, 255, "%s", argv[song_index + 1]);
    printf("NOTE: Received \"%s\"\n", song_name);
    music_loaded = true;
  } else {
    printf("NOTE: No music file specified\n");
  }

  // Load music image if present
  char *cover_image_path = NULL;
  Texture2D cover_texture = {0};
  if (music_loaded) {
   cover_image_path = get_image_file(song_name);
   printf("Cover Image path: %s\n", cover_image_path);
   cover_texture = LoadTexture(cover_image_path);
  }

  // Music stream
  InitAudioDevice();
  Music music = {0};
  float time_played = 0.0f;
  
  if (music_loaded) {
    music = LoadMusicStream(song_name);
    PlayMusicStream(music);
  } else
    printf("NOTE: No music loaded\n");

  // Progress Bar
  Rectangle progress_bar_rect = (Rectangle){screen_width / 2 - screen_width / 3, screen_height / 10 * 8, screen_width / 3 * 2, screen_height / 20};

  // Main loop
  while (!WindowShouldClose()) {
    /* Updating */

    // Play/Pause when space is pressed 
    if (IsKeyPressed(KEY_SPACE)) {
      play = !play;
      last_play = play;
      goto set_stream_state;
    } else if (last_play != play) {
      // Play/Pause when play is changed 
      last_play = play;
      // Is this illegal?
      set_stream_state:
      if (play) ResumeMusicStream(music);
      else PauseMusicStream(music);
    }

    // Seek 10 seconds right
    if (IsKeyPressed(KEY_RIGHT)) {
      if (GetMusicTimePlayed(music) + 10 <= GetMusicTimeLength(music))
        SeekMusicStream(music, GetMusicTimePlayed(music) + 10.0f);
      else
        SeekMusicStream(music, GetMusicTimeLength(music));
    }

    // Seek 10 seconds left
    if (IsKeyPressed(KEY_LEFT)) {
      if (GetMusicTimePlayed(music) - 10 >= 0)
        SeekMusicStream(music, GetMusicTimePlayed(music) - 10.0f);
      else
        SeekMusicStream(music, 0.0f);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      Vector2 mouse_pos = GetMousePosition();

      // Click to seek
      if (collide_point_rect(mouse_pos, progress_bar_rect)) {
        float rect_percentage = (mouse_pos.x - progress_bar_rect.x) / progress_bar_rect.width;
        SeekMusicStream(music, rect_percentage * GetMusicTimeLength(music));
      }
    }

    // Process audio frame after inputs 
    if (music_loaded) UpdateMusicStream(music);

    // Set visual style
    if (style_active != prev_style_active) {
      GuiLoadStyleDefault();

      switch (style_active) {
	      case 1:  GuiLoadStyleDark(); break;
	      case 2:  GuiLoadStyleJungle(); break;
	      case 3:  GuiLoadStyleLavanda(); break;
	      case 4:  GuiLoadStyleCyber(); break;
	      case 5:  GuiLoadStyleTerminal(); break;
	      case 6:  GuiLoadStyleBluish(); break;
        //  case 7:  GuiLoadStyleCandy(); break;
        //	case 8:  GuiLoadStyleAshes(); break;
        //	case 9:  GuiLoadStyleCherry(); break;
        //	case 10: GuiLoadStyleSunny(); break;
        //	case 11: GuiLoadStyleEnefete(); break;
	default: break;
      }
      
      prev_style_active = style_active;
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

      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      if (cover_image_path != NULL)
        DrawTexture(cover_texture, (screen_width - 512) / 2, 100, WHITE);

      if (GuiButton((Rectangle){20, 20, 40, 40}, "#11#")) {
        char last_song_name[sizeof(song_name)];
        strcpy(last_song_name, song_name);
        const char* dialog = file_dialog();
        strcpy(song_name, dialog);
        if (&song_name[0] != &last_song_name[0]) {
          music = LoadMusicStream(song_name); 
          music_loaded = true;
              PlayMusicStream(music);

          cover_image_path = get_image_file(song_name);
          printf("Cover Image path: %s\n", cover_image_path);
          cover_texture = LoadTexture(cover_image_path);
        }
      }

      GuiComboBox((Rectangle){screen_width - 160, 20, 140, 40}, "default;Dark;Jungle;Lavanda;Cyber;Terminal;Bluish", &style_active);

      GuiToggle((Rectangle){screen_width / 2 - screen_width / 16, screen_height / 10 * 9 - font_size_small / 2, 100, font_size_small}, play ? "Play" : "Pause", &play);

      GuiProgressBar(progress_bar_rect, time_progress_str, time_togo_str, &time_played, 0.0f, 1.0f);

    EndDrawing();
  }
  
  free(cover_image_path);

  UnloadMusicStream(music);
  CloseAudioDevice();
  CloseWindow();

  return 0;
}
