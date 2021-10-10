#include <SDL2/SDL_ttf.h>
#include "../include/Render.h"
#include "../include/Window.h"

Window::Window(size_t width, size_t height) {
	window_ = SDL_CreateWindow("Unnamed", SDL_WINDOWPOS_UNDEFINED,
	                           SDL_WINDOWPOS_UNDEFINED, width, height,
	                           SDL_WINDOW_RESIZABLE);
  // window_ = SDL_CreateWindow(
  //     "An SDL2 window",                  // window title
  //     SDL_WINDOWPOS_UNDEFINED,           // initial x position
  //     SDL_WINDOWPOS_UNDEFINED,           // initial y position
  //     width,                               // width, in pixels
  //     height,                               // height, in pixels
  //     SDL_WINDOW_FULLSCREEN_DESKTOP
  // );
  width_ = width;
  height_ = height;
}

size_t Window::GetWidth() const {
	return width_;
}

size_t Window::GetHeight() const {
	return height_;
}

SDL_Window* Window::GetWindow() const {
	return window_;
}

void Window::RenderPresent(const Render& render) const {
 	SDL_RenderPresent(render.GetRender());
}

Window::~Window() {
	SDL_DestroyWindow(window_);
}