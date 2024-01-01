#ifndef Window_H
#define Window_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>

class Window {

 public:
  // Initializes internals
  Window();

  // Creates window
  bool init(int screenWidth, int screenHeight);

  // Handles window events
  void handleEvent(SDL_Event& e);

  // Focuses on window
  void focus();

  // Shows windows contents
  void render();

  void drawpixel(float xm, float ym, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255);
  void clearpixels();
  void update();
  void input();

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
// SDL_bool done;
  std::vector<SDL_FPoint> points;
  std::vector<SDL_Color> colors;
  SDL_Event event{};
  // Window data
  SDL_Window* mWindow;
  SDL_Renderer* mRenderer;
  int mWindowID;

  // Window dimensions
  int mWidth;
  int mHeight;

  // Window focus
  bool mMouseFocus;
  bool mKeyboardFocus;
  bool mFullScreen;
  bool mMinimized;
  bool mShown;
};

#endif