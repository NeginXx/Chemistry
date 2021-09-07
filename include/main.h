struct Window {
	SDL_Renderer* renderer = NULL;
	SDL_Window* window = NULL;
	SDL_Surface* surface = NULL;
	TTF_Font* font = NULL;
	size_t width = 0;
	size_t height = 0;
};

struct Color {
	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;
};