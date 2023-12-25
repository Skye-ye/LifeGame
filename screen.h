#ifndef SCREEN_H
#define SCREEN_H
#include <SDL.h>
#include <ctime>
#include <vector>


class Screen {

	SDL_Window* window{};
	SDL_Renderer* renderer{};
	// SDL_bool done;
	std::vector<SDL_FPoint> points;
	std::vector<SDL_Color> colors;
	SDL_Event event{};
 public:
	Screen();
	void drawpixel(float xm, float ym, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255);
	void clearpixels();
	void update();
	void input();
};

#endif