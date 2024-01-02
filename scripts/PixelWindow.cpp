#include "PixelWindow.h"
#include <iostream>
#include <sstream>

PixelWindow::PixelWindow() : Window(){
  mRefreshSpeed = 17;
}

bool PixelWindow::init(int screenWidth, int screenHeight) {
  // Create window
  mWindow = SDL_CreateWindow(
	  "Life Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	  screenWidth * 2, screenHeight * 2, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
  if (mWindow != nullptr) {
	mMouseFocus = true;
	mKeyboardFocus = true;
	mWidth = screenWidth * 2;
	mHeight = screenHeight * 2;

	// Create mRenderer for window
	mRenderer = SDL_CreateRenderer(
		mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == nullptr) {
	  std::cout << "Renderer could not be created! SDL Error: \n" << SDL_GetError();
	  SDL_DestroyWindow(mWindow);
	  mWindow = nullptr;
	} else {
	  // Initialize mRenderer color
	  SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	  // Set render scale
	  SDL_RenderSetScale(mRenderer, 4, 4);

	  // Grab window identifier
	  mWindowID = SDL_GetWindowID(mWindow);

	  // Flag as opened
	  mShown = true;
	}
  } else {
	std::cout << "Window could not be created! SDL Error: \n" << SDL_GetError();
  }

  return mWindow != nullptr && mRenderer != nullptr;
}

void PixelWindow::resetTitle() {
  std::stringstream caption;
  caption << "Life Game - MouseFocus:" << ((mMouseFocus) ? "On" : "Off")
		  << " KeyboardFocus:" << ((mKeyboardFocus) ? "On" : "Off")
		  << " RefreshSpeed:" << mRefreshSpeed;
  SDL_SetWindowTitle(mWindow, caption.str().c_str());
}

void PixelWindow::render() {
  if (!mMinimized) {
	SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(mRenderer);

	for (long unsigned int i = 0; i < points.size(); i++) {
	  SDL_SetRenderDrawColor(mRenderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
	  SDL_RenderDrawPointF(mRenderer, points[i].x, points[i].y);
	}

	SDL_RenderPresent(mRenderer);
  }
}

void PixelWindow::drawpixel(float xm, float ym, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  points.emplace_back(SDL_FPoint{xm, ym});
  colors.emplace_back(SDL_Color{r, g, b, a});
}

void PixelWindow::clearpixels() {
  points.clear();
}
void PixelWindow::setRefreshSpeed(int refreshRate) {
  mRefreshSpeed = 21 - refreshRate / 5;
  resetTitle();
}
