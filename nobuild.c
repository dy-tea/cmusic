#define NOBUILD_IMPLEMENTATION
#include "nobuild/nobuild.h"

#define CFLAGS "-Wall", "-Wextra", "-Ofast", "-march=native"
#define LIBS "-lraylib", "-lGL", "-lm", "-lpthread", "-ldl", "-lrt", "-lX11"

void build(Cstr path, Cstr target, int release) {
  Cstr raygui = PATH("-I", path_get_current_dir(), "raygui", "src");
  Cstr styles = PATH("-I", path_get_current_dir(), "raygui", "examples", "styles");
#ifndef _WIN32
    CMD("cc", CFLAGS, raygui, styles, LIBS, "-o", NOEXT(target), path);
#else
  printf("NOTE: Windows is untested and may not build");
  CMD("cl.exe", "/Fe.\\src\\", path);
#endif
}

void run(Cstr target) {
  CMD(NOEXT(target));
}

int main(int argc, char **argv) {
  Cstr path = PATH("src", "main.c");
  Cstr target = PATH("target", "cmusic");

  GO_REBUILD_URSELF(argc, argv);

  if (!PATH_EXISTS("target"))
    CMD("mkdir", "target");

  build(path, target, 1);

  run(target);

  return 0;
}
