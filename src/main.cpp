#include "../include/main.h"
#include "../include/Render.h"
#include "../include/Window.h"
#include "../include/App.h"

int main(int argc, const char* argv[]) {
  assert(argc == 2 && "No arguments are passed to the program");
  signal(SIGSEGV, PrintStackInfoAndExit);
  srand(time(NULL));
  // Window window(1800, 900);
  Window window(1200, 1000);
  Render render(window);
  RunApp(window, render, atoi(argv[1]));
}
