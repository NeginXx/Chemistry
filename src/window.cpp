#include <SDL2/SDL_ttf.h>
#include "../include/render.h"
#include "../include/window.h"

Window::Window(size_t width, size_t height) {
	window_ = SDL_CreateWindow("Unnamed", SDL_WINDOWPOS_UNDEFINED,
	                           SDL_WINDOWPOS_UNDEFINED, width, height,
	                           SDL_WINDOW_RESIZABLE);
  width_ = width;
  height_ = height;
}

size_t Window::GetWidth() {
	return width_;
}

size_t Window::GetHeight() {
	return height_;
}

SDL_Window* Window::GetWindow() {
	return window_;
}

void Window::RenderPresent(Render& render) {
 	SDL_RenderPresent(render.GetRender());
}

Window::~Window() {
	SDL_DestroyWindow(window_);
}