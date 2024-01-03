#ifndef Window_H
#define Window_H

#include <SDL.h>
#include <vector>

class Window {

  friend class Texture;

 public:
  // Initializes internals
  Window();

  // Deallocates internals
  ~Window();

  // Creates window
  virtual bool init(int screenWidth, int screenHeight);

  // Reset window title
  virtual void resetTitle();

  // Shows windows contents
  virtual void render();

  // clear window contents
  void clear();

  // Hide window
  void hide();

  // Focuses on window
  void focus();

  // Handles window events
  void handleEvent(SDL_Event &e);

  // Deallocates internals
  void free();

  // Window dimensions
  [[nodiscard]] int getWidth() const;
  [[nodiscard]] int getHeight() const;

  // Window foci
  [[nodiscard]] bool hasMouseFocus() const;
  [[nodiscard]] bool hasKeyboardFocus() const;
  [[nodiscard]] bool isMinimized() const;
  [[nodiscard]] bool isShown() const;

 private:
  std::vector<SDL_FPoint> points;
  std::vector<SDL_Color> colors;

  bool mFullScreen;
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
};

#endif