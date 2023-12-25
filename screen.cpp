#include "screen.h"
#include <iostream>

Screen::Screen() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	std::cout << "SDL could not initialize! SDL Error: \n" << SDL_GetError();
  } else {
	SDL_CreateWindowAndRenderer(1920, 1080, SDL_WINDOW_ALLOW_HIGHDPI, &window, &renderer);
	SDL_RenderSetScale(renderer, 4, 4);
  }
}

void Screen::drawpixel(float xm, float ym, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  points.emplace_back(SDL_FPoint{xm, ym});
  colors.emplace_back(SDL_Color{r, g, b, a});
}

void Screen::clearpixels() {
  points.clear();
}

void Screen::update() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  for (long unsigned int i = 0; i < points.size(); i++) {
	SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
	SDL_RenderDrawPointF(renderer, points[i].x, points[i].y);
  }

  SDL_RenderPresent(renderer);
}

void Screen::input() {
  while (SDL_PollEvent(&event)) {
	switch (event.type) {
	  /* SDL_QUIT event (window close) */
	  case SDL_QUIT: SDL_Quit();
		exit(0);

	  default: break;
	}
  }
}