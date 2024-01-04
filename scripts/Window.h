#ifndef Window_H
#define Window_H

#include "GlobalData.h"
#include <vector>

class Window {

  friend class Texture;

 public:
  // Initialize internals
  Window();

  // Deallocate internals
  ~Window();

  // Create window
  virtual bool init(int screenWidth, int screenHeight);

  // Destroy window
  void free();

  // Reset window title
  virtual void resetTitle();

  // Show windows contents
  virtual void render();

  // clear window contents
  void clear();

  // Hide window
  void hide();

  // Focus on window
  void focus();

  // Handle window events
  void handleEvent(SDL_Event &e);

  // Window dimensions
  [[nodiscard]] int getWidth() const;
  [[nodiscard]] int getHeight() const;

  // Window foci
  [[nodiscard]] bool hasMouseFocus() const;
  [[nodiscard]] bool hasKeyboardFocus() const;
  [[nodiscard]] bool isMinimized() const;
  [[nodiscard]] bool isShown() const;

 protected:
// Window data
  SDL_Window *mWindow;
// Window focus
  bool mMouseFocus;
  bool mKeyboardFocus;
// Window dimensions
  int mWidth;
  int mHeight;
  SDL_Renderer *mRenderer;
  uint32_t mWindowID;
  bool mShown;
  bool mMinimized;
  bool mFullScreen;
};

#endif