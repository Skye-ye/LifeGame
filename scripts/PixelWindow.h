#ifndef LIFEGAME_SCRIPTS_PIXELWINDOW_H_
#define LIFEGAME_SCRIPTS_PIXELWINDOW_H_

#include "Window.h"

// Inherit from Window class to create a window with pixel drawing capability
class PixelWindow : public Window {
  friend class Texture;

 public:
  PixelWindow();
  // override init() to adjust pixel size
  bool init(int screenWidth, int screenHeight) override;

  // override resetTitle() to add refresh speed
  void resetTitle() override;

  // override render() to draw pixels
  void render() override;

  // Draw pixels
  void drawpixel(float xm, float ym, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255);

  // Clear pixels
  void clearpixels();

  // Calculate refresh speed
  void setRefreshSpeed(int refreshRate);
 private:
  // Pixel data
  std::vector<SDL_FPoint> points;
  std::vector<SDL_Color> colors;

  // Refresh speed
  int mRefreshSpeed;
};
#endif //LIFEGAME_SCRIPTS_PIXELWINDOW_H_
