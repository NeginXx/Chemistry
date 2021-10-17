#include "../include/main.h"
#include "../include/Render.h"
#include "../include/Window.h"
#include "../include/App.h"

int main() {
  signal(SIGSEGV, PrintStackInfoAndExit);
  srand(time(NULL));
  // Window window(1800, 900);
  Window window(1000, 1000);
  Render render(window);
  RunApp(window, render);
}