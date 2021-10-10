#include <SDL2/SDL_ttf.h>
#include "../include/Render.h"
#include "../include/Window.h"

const size_t kFontSize = 30;

Render::Render(const Window& window) {
	assert(TTF_Init() >= 0);
	font_ = TTF_OpenFont("font.ttf", kFontSize);
	assert(font_ != NULL);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	render_ = SDL_CreateRenderer(window.GetWindow(), -1,
		                           SDL_RENDERER_ACCELERATED);
}

SDL_Renderer* Render::GetRender() const {
  return render_;
}

void Render::DrawText(const char* text_str, const Point2D<size_t>& coord) {
	SDL_Surface* text = TTF_RenderText_Solid(font_, text_str, SDL_Color{0, 0, 0, 0});
	assert(text != NULL);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(render_, text);
  SDL_Rect dest = {(int)coord.x, (int)coord.y, text->w, text->h};
  SDL_RenderCopy(render_, text_texture, NULL, &dest);
}

void Render::DrawLine(const Point2D<size_t>& a,
	                    const Point2D<size_t>& b,
	   					        const Color& color) {
  SDL_SetRenderDrawColor(render_, color.red, color.green, color.blue, 0);
  SDL_RenderDrawLine(render_, (int)a.x, (int)a.y,
  	                          (int)b.x, (int)b.y);
}

void Render::DrawPoint(const Point2D<size_t>& a,
	   					         const Color& color) {
  SDL_SetRenderDrawColor(render_, color.red, color.green, color.blue, 0);
	SDL_RenderDrawPoint(render_, (int)a.x, (int)a.y);
}

void Render::DrawRectangle(const Point2D<size_t>& left_corner,
		                       const Point2D<size_t>& right_corner,
		                       const Color& color = {}) {
	assert(left_corner.x < right_corner.x);
	assert(left_corner.y < right_corner.y);
	SDL_SetRenderDrawColor(render_, color.red, color.green, color.blue, 0x00);
	SDL_Rect rect = {(int)left_corner.x, (int)left_corner.y,
	                 (int)(right_corner.x - left_corner.x),
	                 (int)(right_corner.y - left_corner.y)};
  SDL_RenderFillRect(render_, &rect);
}

void Render::ChangeBackgroundColor(const Color& color) {
  SDL_SetRenderDrawColor(render_, color.red, color.green, color.blue, 0x00);
  SDL_RenderClear(render_);
}

Render::~Render() {
	SDL_DestroyRenderer(render_);
}