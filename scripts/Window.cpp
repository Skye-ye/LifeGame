#include "Window.h"
#include <iostream>

Window::Window() {
  // Initialize non-existent window
  mWindow = nullptr;
  mRenderer = nullptr;

  mMouseFocus = false;
  mKeyboardFocus = false;
  mFullScreen = false;
  mMinimized = false;
  mWindowID = -1;
  mShown = false;

  mWidth = 0;
  mHeight = 0;
}

bool Window::init(int screenWidth, int screenHeight) {
  // Create window
  mWindow = SDL_CreateWindow(
	  "Life Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	  screenWidth * 2, screenHeight * 2, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
  if (mWindow != nullptr) {
	mMouseFocus = true;
	mKeyboardFocus = true;
	mWidth = screenWidth;
	mHeight = screenHeight;

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

void Window::focus() {
  // Restore window if needed
  if (!mShown) {
	SDL_ShowWindow(mWindow);
  }

  // Move window forward
  SDL_RaiseWindow(mWindow);
}

void Window::render() {
  if (!mMinimized) {
	// Clear screen
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mRenderer);

	// Update screen
	SDL_RenderPresent(mRenderer);
  }
}

void Window::drawpixel(float xm, float ym, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  points.emplace_back(SDL_FPoint{xm, ym});
  colors.emplace_back(SDL_Color{r, g, b, a});
}

void Window::clearpixels() {
  points.clear();
}

void Window::update() {
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(mRenderer);

  for (long unsigned int i = 0; i < points.size(); i++) {
	SDL_SetRenderDrawColor(mRenderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
	SDL_RenderDrawPointF(mRenderer, points[i].x, points[i].y);
  }

  SDL_RenderPresent(mRenderer);
}

void Window::input() {
  while (SDL_PollEvent(&event)) {
	switch (event.type) {
	  /* SDL_QUIT event (window close) */
	  case SDL_QUIT: SDL_Quit();
		exit(0);

	  default: break;
	}
  }
}

void Window::free() {
  if (mWindow != nullptr) {
	SDL_DestroyWindow(mWindow);
  }

  mMouseFocus = false;
  mKeyboardFocus = false;
  mWidth = 0;
  mHeight = 0;
}

int Window::getWidth() const { return mWidth; }

int Window::getHeight() const { return mHeight; }

bool Window::hasMouseFocus() const { return mMouseFocus; }

bool Window::hasKeyboardFocus() const { return mKeyboardFocus; }

bool Window::isMinimized() const { return mMinimized; }

bool Window::isShown() const { return mShown; }