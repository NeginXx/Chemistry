#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "../include/draw.h"

bool SomeEventInQueue(SDL_Event* event_ptr) {
  return SDL_PollEvent(event_ptr);
}

int main() {
  Window window(1400, 900);
  SDL_Event event = {};
  bool running = true
  while (running) {
    if (event.type == SDL_QUIT) {
      break;
    }
    win.RenderEverything();
    win.RenderPresent();
  }
}