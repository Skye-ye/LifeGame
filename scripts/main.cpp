#include <array>
#include <random>
#include <algorithm>
#include <iostream>
#include "Window.h"
#include "Texture.h"
#include "PixelWindow.h"

const int GAME_WIDTH = 720;
const int GAME_HEIGHT = 480;

int UPDATE_RATE = 20;

// import random
std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(1, 104);

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Deal with pausing
int pause();

// Deal with settings
int settings();

// Frees media and shuts down SDL
void close();

// Check if a point is alive
bool isAlive(std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> &game, int x, int y);

// Our custom window
PixelWindow gWindow;

// pause window
Window gPauseWindow;

// Scene textures
Texture gSceneTexture;

int main(int argc, char *argv[]) { // these two parameters are required for SDL
  // Start up SDL and create window
  if (!init()) {
	std::cout << "Failed to initialize!" << std::endl;
  } else {
	// Load media
	if (!loadMedia()) {
	  std::cout << "Failed to load media!" << std::endl;
	} else {

	  // Initialize two arrays, one for the current state of the game and one for the next state
	  std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> display{};
	  std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> swap{};

	  //Create random points
	  for (auto &row : display)
		std::generate(row.begin(), row.end(), []() { return (dist(rng)) % 8 == 0 ? 1 : 0; });

	  // Main loop flag
	  bool quit = false;

	  // Event handler
	  SDL_Event e;

	  // Start game loop
	  while (!quit) {

		// Check for alive points
		for (int i = 0; i < GAME_WIDTH; ++i)
		  for (int j = 0; j < GAME_HEIGHT; ++j)
			swap[i][j] = isAlive(display, i, j) ? 1 : 0;

		//Draw
		for (int i = 0; i < GAME_WIDTH; ++i)
		  for (int j = 0; j < GAME_HEIGHT; ++j)
			if (swap[i][j])
			  gWindow.drawpixel(static_cast<float>(i), static_cast<float>(j));

		// Swap buffers
		std::copy(swap.begin(), swap.end(), display.begin());

		//Display to screen
		gWindow.render();

		// Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
		  // User requests quit
		  if (e.type == SDL_QUIT) {
			quit = true;
		  }

		  // Handle window events
		  gWindow.handleEvent(e);

		  // Handle key presses
		  if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			  // User prompts quit
			  case SDLK_q: quit = true;
				break;

				// Pause the game
			  case SDLK_p:
				if (pause() == 1) {
				  quit = true;
				}
				break;

				// Speed up the game
			  case SDLK_UP: UPDATE_RATE -= 5;
				if (UPDATE_RATE < 5) UPDATE_RATE = 5;
				gWindow.setRefreshSpeed(UPDATE_RATE);
				break;

				// Slow down the game
			  case SDLK_DOWN: UPDATE_RATE += 5;
				if (UPDATE_RATE > 100) UPDATE_RATE = 100;
				gWindow.setRefreshSpeed(UPDATE_RATE);
				break;

			  default: break;
			}
		  }
		}

		// Wait a bit
		SDL_Delay(UPDATE_RATE);

		// Clear pixels
		gWindow.clearpixels();
	  }
	}
  }

  // Free resources and close SDL
  close();
  return 0;
}

bool init() {
  // Initialization flag
  bool success = true;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	std::cout << "SDL could not initialize! SDL Error: \n" << SDL_GetError();
	success = false;
  } else {
	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
	  std::cout << "Warning: Linear texture filtering not enabled!";
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

	// Initialize pause window
	if (!gPauseWindow.init(GAME_WIDTH, GAME_HEIGHT)) {
	  std::cout << "Window could not be created! SDL Error: \n" << SDL_GetError();
	  success = false;
	} else {
	  // Hide pause window to focus on game window
	  gPauseWindow.hide();
	}
  }

  return success;
}

bool loadMedia() {
  // Loading success flag
  bool success = true;

  // Load scene texture
  if (!gSceneTexture.loadFromFile(gPauseWindow, "resources/images/pause.png")) {
	std::cout << "Failed to load window texture!\n";
	success = false;
  }

  return success;
}

int pause() {
  // Pause flag
  bool paused = true;
  int status = 0;

  // Show pause window
  gPauseWindow.focus();

  // Handle events when paused
  while (paused) {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
	  // User prompts quit
	  if (e.type == SDL_QUIT) {
		status = 1;
		paused = false;
	  }

	  // Handle window events
	  gPauseWindow.handleEvent(e);
	  gWindow.handleEvent(e);

	  // Handle key presses
	  if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		  // User prompts quit
		  case SDLK_q: status = 1;
			paused = false;
			break;

			// User prompts menu
		  case SDLK_s: if (settings() == 1) status = 1;
			break;

			// Resume game
		  case SDLK_r: paused = false;
			break;

		  default: break;
		}
	  }
	}
	gPauseWindow.clear();
	gSceneTexture.render(gPauseWindow, 0, 0);
	gPauseWindow.render();

	// Check all windows
	bool allWindowsClosed = true;
	if (gWindow.isShown() || gPauseWindow.isShown()) {
	  allWindowsClosed = false;
	}

	// If all windows are closed, quit
	if (allWindowsClosed) {
	  status = 1;
	  paused = false;
	}
  }

  // Hide pause window and return to game window
  gPauseWindow.hide();
  gWindow.focus();

  // Return to game
  return status;
}

int settings() {
  // Settings flag
  bool done = false;
  int status = 0;

  // Handle setting events
  while (!done) {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
	  // User prompts quit
	  if (e.type == SDL_QUIT) {
		status = 1;
		done = true;
	  }

	  // Handle window events
	  gWindow.handleEvent(e);

	  // Handle user input
	  if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		  // Return to pause menu
		  case SDLK_RETURN: done = true;
			break;
		  default: break;
		}
	  }
	}
  }

  // Return to pause menu
  return status;
}

void close() {
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
