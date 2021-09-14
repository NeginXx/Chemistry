#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "../include/main.h"
#include "../include/render.h"
#include "../include/window.h"
#include "../include/app.h"

Point2D<size_t> operator+(const Point2D<size_t>& lhs,
                          const Point2D<size_t>& rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}

Point2D<float> operator+(const Point2D<float>& lhs,
                         const Point2D<float>& rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}

bool SomeEventInQueue(SDL_Event* event_ptr) {
  return SDL_PollEvent(event_ptr);
}

int main() {
  clock_t t = clock();
  Window window(1400, 900);
  Render render(window);
  SDL_Event event = {};
  bool is_running = true;
  while (is_running) {
    DrawEverything(render);
    window.RenderPresent(render);
    if ((double)(clock() - t) / CLOCKS_PER_SEC > 40.0) {
      break;
    }
    // SDL_Delay(50);
    while (SomeEventInQueue(&event)) {
      if (event.type == SDL_QUIT) {
        is_running = false;
        break;
      }
    }
  }
}