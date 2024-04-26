#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#define CFLAGS "-Wall", "-Wextra", "-std=c99", "-lraylib", "-lGL", "-lm", "-lpthread", "-ldl", "-lrt", "-lX11"

void build(Cstr path) {
#ifndef _WIN32
  CMD("cc", CFLAGS, "-o", NOEXT(path), path);
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
