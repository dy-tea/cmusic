#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#define CFLAGS "-Wall", "-Wextra", "-std=gnu99"
#define LIBS "-lraylib", "-lGL", "-lm", "-lpthread", "-ldl", "-lrt", "-lX11"

void build(Cstr path) {
  Cstr inc_dir = PATH("-I", path_get_current_dir(), "include");
#ifndef _WIN32
  CMD("cc", CFLAGS, inc_dir, LIBS, "-o", NOEXT(path), path);
#else
  CMD("cl.exe", "/Fe.\\src\\", path);
#endif
}

void run(Cstr path) {
  CMD(NOEXT(path));
}

int main(int argc, char **argv) {
  Cstr path = PATH("src", "main.c");

  GO_REBUILD_URSELF(argc, argv);

  build(path);
  run(path);

  return 0;
}
