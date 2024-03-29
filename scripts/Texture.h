#ifndef LIFEGAME_SCRIPTS_Texture_H
#define LIFEGAME_SCRIPTS_Texture_H

#include "GlobalData.h"
#include "Window.h"

class Texture {
 public:
  // Initializes variables
  Texture();

  // Deallocates memory
  ~Texture();

  // Loads image at specified path
  bool loadFromFile(Window& window, const std::string& path);

#if defined(SDL_TTF_MAJOR_VERSION)
  // Creates image from font string
  bool loadFromRenderedText(Window& window, const std::string& textureText, SDL_Color textColor);
#endif

  // Deallocates texture
  void free();

  // Set color modulation
  void setColor(Uint8 red, Uint8 green, Uint8 blue);

  // Set blending
  void setBlendMode(SDL_BlendMode blending);

  // Set alpha modulation
  void setAlpha(Uint8 alpha);

  // Renders texture at given point
  void render(Window& window, int x, int y, SDL_Rect *clip = nullptr, double angle = 0.0,
			  SDL_Point *center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

  // Gets image dimensions
  [[nodiscard]] int getWidth() const;
  [[nodiscard]] int getHeight() const;

 private:
  // The actual hardware texture
  SDL_Texture *mTexture;

  // Image dimensions
  int mWidth;
  int mHeight;
};

#endif //LIFEGAME_SCRIPTS_Texture_H