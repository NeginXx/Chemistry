#pragma once

struct SDL_Window;

class Window {
 public:
	Window(size_t width, size_t height);
	size_t GetWidth() const;
	size_t GetHeight() const;
	SDL_Window* GetWindow() const;
	void RenderPresent(const Render& render) const;
	~Window();

 private:
	size_t width_ = 0;
 	size_t height_ = 0;
	SDL_Window* window_ = nullptr;
};