#include <SDL2/SDL.h>
#include "../include/Events.h"

bool IsSomeEventInQueue(Event* event) {
  SDL_Event sdl_event;
  if (!SDL_PollEvent(&sdl_event)) {
    return false;
  }
  switch (sdl_event.type) {
    case SDL_QUIT:
      event->type = kQuit;
      break;
    case SDL_KEYDOWN:
      event->type = kKeyboardClick;
      event->value.scancode = sdl_event.key.keysym.scancode;
      break;
    case SDL_MOUSEBUTTONDOWN:
      event->type = kMouseClick;
      assert(sdl_event.button.x >= 0);
      assert(sdl_event.button.y >= 0);
      event->value.coordinate = Point2D<size_t>{(size_t)sdl_event.button.x, (size_t)sdl_event.button.y};
      break;
    default:
      return false;
  }
  return true;
}