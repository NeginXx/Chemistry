#include "../include/main.h"
#include "../include/render.h"
#include "../include/window.h"
#include "../include/app.h"

int main() {
  signal(SIGSEGV, PrintStackInfoAndExit);
  srand(time(NULL));
  Window window(1500, 1000);
  Render render(window);
  RunApp(window, render);
}