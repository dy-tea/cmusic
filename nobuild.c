#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#define CFLAGS_DEBUG "-Wall", "-Wextra", "-std=gnu99"
#define CFLAGS_RELEASE "-std=gnu99", "-Ofast", "-march=native"
#define LIBS "-lraylib", "-lGL", "-lm", "-lpthread", "-ldl", "-lrt", "-lX11"

void build(Cstr path, Cstr target, int release) {
  Cstr inc_dir = PATH("-I", path_get_current_dir(), "include");
#ifndef _WIN32
  if (release == 1)
    CMD("cc", CFLAGS_RELEASE, inc_dir, LIBS, "-o", NOEXT(target), path);
  else
    CMD("cc", CFLAGS_DEBUG, inc_dir, LIBS, "-o", NOEXT(target), path);
#else
  printf("NOTE: Windows is untested and may not build");
  CMD("cl.exe", "/Fe.\\src\\", path);
#endif
}

void build_release(Cstr path, Cstr target) {
  Cstr inc_dir = PATH("-I", path_get_current_dir(), "include");
#ifndef _WIN32
  CMD("cc", CFLAGS_RELEASE, inc_dir, LIBS, "-o", NOEXT(target), path);
#else
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

  if (argc > 1) {
    printf("NOTE: Building in release mode"); 
    build(path, target, 1);
  } else {
    printf("NOTE: Building in debug mode"); 
    build(path, target, 0);
  }

  run(target);

  return 0;
}
