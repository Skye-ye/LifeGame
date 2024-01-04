#include "Window.h"
#include <iostream>
#include <sstream>

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

Window::~Window() {
  // Deallocate
  free();
}

bool Window::init(int screenWidth, int screenHeight) {
  // Create window
  mWindow = SDL_CreateWindow(
	  "Life Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	  screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
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

void Window::free() {
  if (mRenderer != nullptr) {
	SDL_DestroyRenderer(mRenderer);
  }
  if (mWindow != nullptr) {
	SDL_DestroyWindow(mWindow);
  }

  mMouseFocus = false;
  mKeyboardFocus = false;
  mWidth = 0;
  mHeight = 0;
}

void Window::handleEvent(SDL_Event &e) {
  // If an event was detected for this window
  if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID) {
	// Caption update flag
	bool updateCaption = false;

	switch (e.window.event) {
	  // Window appeared
	  case SDL_WINDOWEVENT_SHOWN: mShown = true;
		break;

		// Window disappeared
	  case SDL_WINDOWEVENT_HIDDEN: mShown = false;
		break;

		// Get new dimensions and repaint
	  case SDL_WINDOWEVENT_SIZE_CHANGED: mWidth = e.window.data1;
		mHeight = e.window.data2;
		SDL_RenderPresent(mRenderer);
		break;

		// Repaint on expose
	  case SDL_WINDOWEVENT_EXPOSED: SDL_RenderPresent(mRenderer);
		break;

		// Mouse enter
	  case SDL_WINDOWEVENT_ENTER: mMouseFocus = true;
		updateCaption = true;
		break;

		// Mouse exit
	  case SDL_WINDOWEVENT_LEAVE: mMouseFocus = false;
		updateCaption = true;
		break;

		// Keyboard focus gained
	  case SDL_WINDOWEVENT_FOCUS_GAINED: mKeyboardFocus = true;
		updateCaption = true;
		break;

		// Keyboard focus lost
	  case SDL_WINDOWEVENT_FOCUS_LOST: mKeyboardFocus = false;
		updateCaption = true;
		break;

		// Window minimized
	  case SDL_WINDOWEVENT_MINIMIZED: mMinimized = true;
		break;

		// Window maximized
	  case SDL_WINDOWEVENT_MAXIMIZED: mMinimized = false;
		break;

		// Window restored
	  case SDL_WINDOWEVENT_RESTORED: mMinimized = false;
		break;

		// Hide on close
	  case SDL_WINDOWEVENT_CLOSE: SDL_HideWindow(mWindow);
		break;
	}

	// Update window caption with new data
	if (updateCaption) {
	  resetTitle();
	}
  }
	// Enter exit full screen on return key
  else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
	if (mFullScreen) {
	  SDL_SetWindowFullscreen(mWindow, 0);
	  mFullScreen = false;
	} else {
	  SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	  mFullScreen = true;
	  mMinimized = false;
	}
  }
}

void Window::resetTitle() {
  std::stringstream caption;
  caption << "Life Game - MouseFocus:" << ((mMouseFocus) ? "On" : "Off")
		  << " KeyboardFocus:" << ((mKeyboardFocus) ? "On" : "Off");
  SDL_SetWindowTitle(mWindow, caption.str().c_str());
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
	// Update screen
	SDL_RenderPresent(mRenderer);
  }
}

void Window::clear() {
  // Clear screen
  if (!mMinimized) {
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mRenderer);
  }
}

void Window::hide() {
  SDL_HideWindow(mWindow);
  mShown = false;
}

int Window::getWidth() const { return mWidth; }

int Window::getHeight() const { return mHeight; }

bool Window::hasMouseFocus() const { return mMouseFocus; }

bool Window::hasKeyboardFocus() const { return mKeyboardFocus; }

bool Window::isMinimized() const { return mMinimized; }

bool Window::isShown() const { return mShown; }
