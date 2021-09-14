#include <SDL2/SDL_ttf.h>
#include "../include/main.h"
#include "../include/render.h"
#include "../include/window.h"

Render::Render(Window& window) {
	assert(TTF_Init() >= 0);
	font_ = TTF_OpenFont("font.ttf", 30);
	assert(font_ != NULL);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	render_ = SDL_CreateRenderer(window.GetWindow(), -1,
		                           SDL_RENDERER_ACCELERATED);
}

SDL_Renderer* Render::GetRender() {
  return render_;
}

void Render::DrawText(const char* text_str, Point2D<size_t> coord) {
	SDL_Surface* text = TTF_RenderText_Solid(font_, text_str, {0, 0, 0});
	assert(text != NULL);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(render_, text);
  SDL_Rect dest = {(int)coord.x, (int)coord.y, text->w, text->h};
  SDL_RenderCopy(render_, text_texture, NULL, &dest);
}

void Render::DrawLine(Point2D<size_t> a,
	                    Point2D<size_t> b,
	   					        Color color) {
  SDL_SetRenderDrawColor(render_, color.red, color.green, color.blue, 0);
  SDL_RenderDrawLine(render_, (int)a.x, (int)a.y,
  	                          (int)b.x, (int)b.y);
}

void Render::DrawPoint(Point2D<size_t> a,
	   					         Color color) {
  SDL_SetRenderDrawColor(render_, color.red, color.green, color.blue, 0);
	SDL_RenderDrawPoint(render_, (int)a.x, (int)a.y);
}

void Render::DrawRectangle(Point2D<size_t> left_corner,
		                       size_t width, size_t height,
		                       Color color = {}) {
	SDL_SetRenderDrawColor(render_, color.red, color.green, color.blue, 0x00);
	SDL_Rect rect = {(int)left_corner.x, (int)left_corner.y,
	                 (int)width, (int)height};
  SDL_RenderFillRect(render_, &rect);
}

void Render::ChangeBackgroundColor(Color color) {
  SDL_SetRenderDrawColor(render_, color.red, color.green, color.blue, 0x00);
  SDL_RenderClear(render_);
}

Render::~Render() {
	SDL_DestroyRenderer(render_);
}