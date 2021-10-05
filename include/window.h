#pragma once

struct SDL_Window;

class Window {
 public:
	Window(size_t width, size_t height);
	size_t GetWidth();
	size_t GetHeight();
	SDL_Window* GetWindow();
	void RenderPresent(Render& render);
	~Window();

 private:
	size_t width_ = 0;
 	size_t height_ = 0;
	SDL_Window* window_ = nullptr;
};