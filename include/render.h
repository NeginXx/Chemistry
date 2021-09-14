#pragma once

struct Window;
struct SDL_Renderer;
struct _TTF_Font;

class Render {
 public:
 	Render(Window& window);
 	SDL_Renderer* GetRender();
  void DrawText(const char* text_str, Point2D<size_t> coord);
	void DrawLine(Point2D<size_t> a,
		            Point2D<size_t> b,
		   					Color color = {});
	void DrawPoint(Point2D<size_t> a,
		   					 Color color = {});
	void DrawRectangle(Point2D<size_t> left_corner,
			               size_t width, size_t height,
			               Color color);
	void ChangeBackgroundColor(Color color);
  ~Render();

 private:
 	SDL_Renderer* render_ = nullptr;
	_TTF_Font* font_ = nullptr;
};