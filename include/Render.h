#pragma once
#include "main.h"

struct Window;
struct SDL_Renderer;
struct _TTF_Font;

class Render {
 public:
 	Render(const Window& window);
 	SDL_Renderer* GetRender() const;
  void DrawText(const char* text_str, const Point2D<size_t>& coord);
	void DrawLine(const Point2D<size_t>& a,
		            const Point2D<size_t>& b,
		   					const Color& color = {});
	void DrawPoint(const Point2D<size_t>& a,
		   					 const Color& color = {});
	void DrawRectangle(const Point2D<size_t>& left_corner,
			               const Point2D<size_t>& right_corner,
			               const Color& color);
	void ChangeBackgroundColor(const Color& color);
  ~Render();

 private:
 	SDL_Renderer* render_ = nullptr;
	_TTF_Font* font_ = nullptr;
};