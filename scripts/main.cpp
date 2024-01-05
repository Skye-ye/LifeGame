#include <array>
#include <random>
#include <algorithm>
#include <iostream>
#include "GlobalData.h"
#include "Window.h"
#include "Texture.h"
#include "PixelWindow.h"

//Initial update rate
int UPDATE_RATE = 20;

// Initial conditions
int LOWER_BOUND = 2;
int UPPER_BOUND = 3;

// Declare global font
TTF_Font *gFont;

// import random
std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(1, 104);

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Generate random pixels
void generateRandomPixels(std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> &game);

// Deal with pausing
int pause();

// Deal with settings
int settings();

// Frees media and shuts down SDL
void close();

// Check if a point is alive
bool isAlive(std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> &game, int x, int y);

// Initialize two arrays, one for the current state of the game and one for the next state
std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> display{};
std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> swap{};

// Our custom window
PixelWindow gWindow;

// Pause window
Window gPauseWindow;

// Setting window
Window gSettingWindow;

// Pause textures
Texture gPauseTexture;

// Setting header textures
Texture gSettingHeaderTextTexture;

// Input conditions texture
Texture gFirstConditionTexture;
Texture gSecondConditionTexture;

// Input text texture
Texture gFirstInputTextTexture;
Texture gSecondInputTextTexture;

int main(int argc, char *argv[]) { // these two parameters are required for SDL
  // Start up SDL and create window
  if (!init()) {
	std::cout << "Failed to initialize!" << std::endl;
	return 1;
  } else {
	// Load media
	if (!loadMedia()) {
	  std::cout << "Failed to load media!" << std::endl;
	  return 1;
	} else {

	  //Create random points
	  generateRandomPixels(display);

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
				if (pause() == 1)
				  quit = true;
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
	if (!gPauseWindow.init(GAME_WIDTH / 2, GAME_HEIGHT / 2)) {
	  std::cout << "Window could not be created! SDL Error: \n" << SDL_GetError();
	  success = false;
	} else {
	  // Hide pause window to focus on game window
	  gPauseWindow.hide();
	}

	// Initialize setting window
	if (!gSettingWindow.init(GAME_WIDTH / 2, GAME_HEIGHT / 2)) {
	  std::cout << "Window could not be created! SDL Error: \n" << SDL_GetError();
	  success = false;
	} else {
	  // Hide setting window to focus on game window
	  gSettingWindow.hide();
	}
  }

  return success;
}

bool loadMedia() {
  // Loading success flag
  bool success = true;

  // Load scene texture
  if (!gPauseTexture.loadFromFile(gPauseWindow, "resources/images/pause.png")) {
	std::cout << "Failed to load window texture!" << std::endl;
	success = false;
  } else {
	// Open the font
	gFont = TTF_OpenFont("resources/fonts/Arial Unicode.ttf", 40);
	if (gFont == nullptr) {
	  std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
	  success = false;
	} else {
	  // Render setting header text
	  SDL_Color textColor = {0, 0, 0, 0xFF};
	  if (!gSettingHeaderTextTexture.loadFromRenderedText(gSettingWindow, "Setting", textColor)) {
		std::cout << "Failed to render text texture!" << std::endl;
		success = false;
	  }

	  // Render first condition text
	  if (!gFirstConditionTexture.loadFromRenderedText(gSettingWindow, "   Less than     nears to die ", textColor)) {
		std::cout << "Failed to render text texture!" << std::endl;
		success = false;
	  }

	  // Render second condition text
	  if (!gSecondConditionTexture.loadFromRenderedText(gSettingWindow, "   More than    nears to die ", textColor)) {
		std::cout << "Failed to render text texture!" << std::endl;
		success = false;
	  }
	}
  }

  return success;
}

void generateRandomPixels(std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> &game) {
  // Create random points
  for (auto &row : game)
	std::generate(row.begin(), row.end(),
				  []() { return (dist(rng)) % 8 == 0 ? 1 : 0; });
}

int pause() {
  // Pause flag
  bool pauseDone = false;
  int pauseStatus = 0;
  SDL_Event e;


  // Show pause window
  gPauseWindow.focus();

  // Handle events when paused
  while (!pauseDone) {
	while (SDL_PollEvent(&e) != 0) {
	  // User prompts quit
	  if (e.type == SDL_QUIT) {
		pauseStatus = 1;
		pauseDone = true;
	  }

	  // Handle window events
	  gPauseWindow.handleEvent(e);
	  gWindow.handleEvent(e);

	  // Handle key presses
	  if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		  // User prompts quit
		  case SDLK_q: pauseStatus = 1;
			pauseDone = true;
			break;

			// User prompts menu
		  case SDLK_s: gPauseWindow.hide();
			if (settings() == 1) {
			  pauseStatus = 1;
			  pauseDone = true;
			} else {
			  gPauseWindow.focus();
			}
			break;

			// Resume game
		  case SDLK_r:
			// Press ctrl + r to restart
			if (SDL_GetModState() & KMOD_CTRL) {
			  // Reset conditions
			  LOWER_BOUND = 2;
			  UPPER_BOUND = 3;
			  UPDATE_RATE = 20;
			  gWindow.setRefreshSpeed(UPDATE_RATE);
			  generateRandomPixels(display);
			}
			pauseDone = true;
			break;

		  default: break;
		}
	  }
	}
	// Clear and render pause window
	gPauseWindow.clear();
	gPauseTexture.render(gPauseWindow, (gPauseWindow.getWidth() - gPauseTexture.getWidth()) / 2,
						 (gPauseWindow.getHeight() - gPauseTexture.getHeight()) / 2);
	gPauseWindow.render();

	// Check all windows
	bool allWindowsClosed = true;
	if (gWindow.isShown() || gPauseWindow.isShown()) {
	  allWindowsClosed = false;
	}

	// If all windows are closed, quit
	if (allWindowsClosed) {
	  pauseStatus = 1;
	  pauseDone = true;
	}
  }

  // Hide pause window and return to game window
  gPauseWindow.hide();
  gWindow.focus();

  // Return to game
  return pauseStatus;
}

int settings() {
  // Settings flag
  bool settingDone = false;
  int settingStatus = 0;
  bool firstConditionEntered = false;
  bool secondConditionEntered = false;
  SDL_Event e;

  // Set text color as black
  SDL_Color textColor = {0, 0, 0, 0xFF};

  // The current input text
  std::string inputText;
  std::string inputText1 = std::to_string(LOWER_BOUND);
  std::string inputText2 = std::to_string(UPPER_BOUND);
  inputText = inputText1;
  gFirstInputTextTexture.loadFromRenderedText(gSettingWindow, inputText1, textColor);
  gSecondInputTextTexture.loadFromRenderedText(gSettingWindow, inputText2, textColor);

  // Show setting window
  gSettingWindow.focus();

  // Enable text input
  SDL_StartTextInput();

  // Handle setting events
  while (!settingDone) {
	// The rerender text flag
	bool renderText = false;

	while (SDL_PollEvent(&e) != 0) {
	  // User prompts quit
	  if (e.type == SDL_QUIT) {
		settingStatus = 1;
		settingDone = true;
	  }

	  // Handle window events
	  gWindow.handleEvent(e);
	  gSettingWindow.handleEvent(e);

	  // Handle user input
	  if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		  // User prompts quit
		  case SDLK_q: settingStatus = 1;
			settingDone = true;
			break;

			// Handle backspace
		  case SDLK_BACKSPACE:
			if (!inputText.empty()) {
			  //lop off character
			  inputText.pop_back();
			  renderText = true;
			}
			break;

			// Handle enter
		  case SDLK_RETURN:
			if (!inputText.empty()) {
			  if (!firstConditionEntered) {
				LOWER_BOUND = std::stoi(inputText);
				firstConditionEntered = true;
				inputText = inputText2;
			  } else if (!secondConditionEntered) {
				UPPER_BOUND = std::stoi(inputText);
				secondConditionEntered = true;
			  }
			}
			break;

			// Handle copy
		  case SDLK_c:
			if (SDL_GetModState() & KMOD_CTRL) {
			  SDL_SetClipboardText(inputText.c_str());
			}
			break;

			// Handle paste
		  case SDLK_v:
			if (SDL_GetModState() & KMOD_CTRL) {
			  char *text = SDL_GetClipboardText();
			  if (text != nullptr) {
				inputText = text;
				SDL_free(text);
				renderText = true;
			  }
			}
			break;

		  default: break;
		}

		// Special text input event
	  } else if (e.type == SDL_TEXTINPUT) {
		// Only single-digit numbers
		if (inputText.empty()) {
		  //Not copy or pasting
		  if (!(SDL_GetModState() & KMOD_CTRL &&
			  (e.text.text[0] == 'c' || e.text.text[0] == 'C' ||
				  e.text.text[0] == 'v' || e.text.text[0] == 'V'))) {
			//Append character
			inputText += e.text.text;
			renderText = true;
		  }
		}
	  }
	}
	// Rerender text if needed
	if (renderText) {
	  //Text is not empty
	  if (!inputText.empty()) {
		//Render new text
		if (!firstConditionEntered) {
		  inputText1 = inputText;
		} else {
		  inputText2 = inputText;
		}
		gFirstInputTextTexture.loadFromRenderedText(gSettingWindow, inputText1, textColor);
		gSecondInputTextTexture.loadFromRenderedText(gSettingWindow, inputText2, textColor);
	  } else {
		//Render space texture
		if (!firstConditionEntered) {
		  gFirstInputTextTexture.loadFromRenderedText(gSettingWindow, " ", textColor);
		}
		gSecondInputTextTexture.loadFromRenderedText(gSettingWindow, " ", textColor);
	  }
	}
	// Clear and render setting window
	gSettingWindow.clear();

	// Always render Header
	gSettingHeaderTextTexture.render(gSettingWindow,
									 (gSettingWindow.getWidth() - gSettingHeaderTextTexture.getWidth()) / 2, 0);

	// Render first condition if entered
	gFirstConditionTexture.render(gSettingWindow,
								  (gSettingWindow.getWidth() - gFirstConditionTexture.getWidth()) / 2,
								  (gSettingWindow.getHeight() - gFirstConditionTexture.getHeight()) / 2 - 50);
	gFirstInputTextTexture.render(gSettingWindow,
								  (gSettingWindow.getWidth() - gFirstInputTextTexture.getWidth()) / 2,
								  (gSettingWindow.getHeight() - gFirstInputTextTexture.getHeight()) / 2 - 50);

	// Render second condition if entered
	if (firstConditionEntered) {
	  gSecondConditionTexture.render(gSettingWindow,
									 (gSettingWindow.getWidth() - gSecondConditionTexture.getWidth()) / 2,
									 (gSettingWindow.getHeight() - gSecondConditionTexture.getHeight()) / 2 + 50);
	  gSecondInputTextTexture.render(gSettingWindow,
									 (gSettingWindow.getWidth() - gSecondInputTextTexture.getWidth()) / 2,
									 (gSettingWindow.getHeight() - gSettingHeaderTextTexture.getHeight()) / 2 + 50);
	}

	// Render setting window
	gSettingWindow.render();

	// Check all windows
	bool allWindowsClosed = true;
	if (gWindow.isShown() || gSettingWindow.isShown()) {
	  allWindowsClosed = false;
	}

	// If all windows are closed, quit
	if (allWindowsClosed) {
	  settingStatus = 1;
	  settingDone = true;
	}

	if (firstConditionEntered && secondConditionEntered) {
	  settingDone = true;
	}
  }

  // Disable text input
  SDL_StopTextInput();

  // Hide setting window
  gSettingWindow.hide();

  // Return status to pause
  return settingStatus;
}

void close() {
  // Free global font
  TTF_CloseFont(gFont);
  gFont = nullptr;

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

  // alive and fewer than LOWER_BOUND die
  if (game[x][y] == 1 && alive < LOWER_BOUND) return false;
  // alive and 2 or 3 then live
  if (game[x][y] == 1 && (alive == 2 || alive == 3)) return true;
  // more than UPPER_BOUND live die
  if (alive > UPPER_BOUND) return false;
  // 3 alive and point is dead, come to life
  if (game[x][y] == 0 && alive == 3) return true;

  return false;
}
