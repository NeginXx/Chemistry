class Window {
 public:
 	Window(size_t width, size_t height) {
	  assert(SDL_Init(SDL_INIT_VIDEO) >= 0);
	  assert(TTF_Init() >= 0);
	  font_ = TTF_OpenFont("font.ttf", 50);
	  assert(font != NULL);
	  window_ = SDL_CreateWindow("Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
	  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
	  surface_ = NULL;
	  width_ = width;
	  height_ = height;
 	}

 	void RenderPresent() {
 		SDL_RenderPresent(renderer_);
 	}

 	void RenderEverything() {
	  Color kLightPink = {255, 153, 204},
	             kBlue = {0, 0, 255},
	            kBlack = {0, 0, 0},
	            kWhite = {255, 255, 255};

	  ChangeWindowColor(kBlue);
	  GraphFrame frame = {kLightPink, 100, 100, 500, 300};
	  DrawGraphFrame(win, &frame);
	  Axes axes = {kBlack, &frame, 10, 20, 250, 150};
	  DrawAxes(win, &axes);
	  DrawFunction(win, &axes, ParabolaFunc);

	  frame = {kLightPink, 650, 450, 600, 200};
	  DrawGraphFrame(win, &frame);
	  axes = {kBlack, &frame, 5, 3, 200, 150};
	  DrawAxes(win, &axes);
	  DrawFunction(win, &axes, SinFunc);

	  static float angle = -0.01;
	  angle += 0.01;
	  if (angle > 100) {
	  	angle = 0.0;
	  }
	  Vector vec = {kWhite, &axes, 1, 1, 1, 1};
	  RotateVector(win, &vec, angle);
	  DrawVector(win, &vec);
	}

	void DrawLine (Point2D<size_t>& a,
	               Point2D<size_t>& b,
	   					   Color color = {}) {
	  SDL_SetRenderDrawColor(renderer_, color.red, color.green, color.blue, 0);
	  SDL_RenderDrawLine(renderer_, (int)a.x, (int)a.y,
	  	                            (int)b.x, (int)b.y);
	}

	void DrawRectangle(Point2D<size_t> left_corner,
		                 size_t width, size_t height,
		                 Color color = {}) {
		SDL_Rect rect = {left_corner.x, left_corner.y,
		                 width, height};
	  SDL_RenderDrawRect(renderer_, rect);
	}

	~Window() {
		SDL_DestroyRenderer(renderer_);
		SDL_DestroyWindow(window_);
	}

 private:
 	size_t width_ = 0;
 	size_t height_ = 0;
	SDL_Window* window_ = NULL;
	SDL_Renderer* renderer_ = NULL;
	TTF_Font* font_ = NULL;

	void ChangeWindowColor(Color color) {
	  SDL_SetRenderDrawColor(renderer_, color.red, color.green, color.blue, 0x00);
	  SDL_RenderClear(renderer_);
	}

	void DrawText(const char* text_str, size_t x, size_t y) {
		SDL_Surface* text = TTF_RenderText_Solid(font_, text_str, {0, 0, 0});
		assert(text != NULL);
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer_, text);
	  SDL_Rect dest = {(int)x, (int)y, text->w, text->h};
	  SDL_RenderCopy(renderer_, text_texture, NULL, &dest);
	}
};