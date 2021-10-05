#include "../include/main.h"
#include "../include/render.h"
#include "../include/window.h"
#include "../include/app.h"

int main() {
  signal(SIGSEGV, PrintStackInfoAndExit);
  srand(time(NULL));
  Window window(1400, 900);
  Render render(window);
  RunApp(window, render);
}