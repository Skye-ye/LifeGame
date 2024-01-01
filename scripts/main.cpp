#include <array>
#include <random>
#include <algorithm>
#include <iostream>
#include "Window.h"
#include "LTexture.h"

const int GAME_WIDTH = 720;
const int GAME_HEIGHT = 480;

// import random
std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(1, 104);

bool init();

Window gWindow;

void close();

bool isAlive(std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> &game, int x, int y);

int main(int argc, char *argv[]) {
  // Start up SDL and create window
  if (!init()) {
	printf("Failed to initialize!\n");
  } else {
	// Initialize two arrays, one for the current state of the game and one for the next state
	std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> display{};
	std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> swap{};

	//Create random points
	for (auto &row : display)
	  std::generate(row.begin(), row.end(), []() { return (dist(rng)) % 8 == 0 ? 1 : 0; });

	// Start game loop
	while (true) {
	  // Check for alive points
	  for (int i = 0; i < GAME_WIDTH; ++i)
		for (int j = 0; j < GAME_HEIGHT; ++j)
		  swap[i][j] = isAlive(display, i, j) ? 1 : 0;

	  //Draw
	  for (int i = 0; i < GAME_WIDTH; ++i)
		for (int j = 0; j < GAME_HEIGHT; ++j) {
		  if (swap[i][j]) {
			gWindow.drawpixel(static_cast<float>(i), static_cast<float>(j));
		  }
		}

	  // Swap buffers
	  std::copy(swap.begin(), swap.end(), display.begin());

	  //Display to screen
	  gWindow.update();
	  SDL_Delay(20);
	  gWindow.input();
	  gWindow.clearpixels();
	}
  }
  close();
  return 0;
}

bool init() {
  // Initialization flag
  bool success = true;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
	success = false;
  } else {
	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
	  printf("Warning: Linear texture filtering not enabled!");
	}

	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
	  std::cout << "SDL_image could not initialize! SDL_image Error: \n" << IMG_GetError();
	  success = false;
	}

	// Initialize SDL_ttf
	if (TTF_Init() == -1) {
	  std::cout << "SDL_ttf could not initialize! SDL_ttf Error: \n" << TTF_GetError();
	  success = false;
	}

	// Create window
	if (!gWindow.init(GAME_WIDTH, GAME_HEIGHT)) {
	  std::cout << "Window could not be created! SDL Error: \n" << SDL_GetError();
	  success = false;
	}
  }

  return success;
}

void close() {
  // Destroy windows
  gWindow.free();

  // Quit SDL subsystems
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

bool isAlive(std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> &game, const int x, const int y) {
  int alive = 0;
  // testing the left
  if (x > 0 && game[x - 1][y] == 1) alive += 1;
  // testing the right
  if (x < GAME_WIDTH && game[x + 1][y] == 1) alive += 1;
  // testing top
  if (y > 0 && game[x][y - 1] == 1) alive += 1;
  // testing bottom
  if (y < GAME_HEIGHT && game[x][y + 1] == 1) alive += 1;

  // testing top left
  if (y > 0 && x > 0 && game[x - 1][y - 1] == 1) alive += 1;
  // testing top right
  if (y > 0 && x < GAME_WIDTH && game[x + 1][y - 1] == 1)alive += 1;
  // testing bottom left
  if (y < GAME_HEIGHT && x > 0 && game[x - 1][y + 1] == 1) alive += 1;
  // testing bottom right
  if (y < GAME_HEIGHT && x < GAME_WIDTH && game[x + 1][y + 1] == 1) alive += 1;

  // alive and fewer than 2 die
  if (game[x][y] == 1 && alive < 2) return false;
  // alive and 2 or 3 then live
  if (game[x][y] == 1 && (alive == 2 || alive == 3)) return true;
  // more than 3 live die
  if (alive > 3) return false;
  // 3 alive and point is dead, come to life
  if (game[x][y] == 0 && alive == 3) return true;

  return false;
}
