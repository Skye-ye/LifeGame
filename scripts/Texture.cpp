#include "Texture.h"
#include <iostream>

Texture::Texture() {
  // Initialize
  mTexture = nullptr;
  mWidth = 0;
  mHeight = 0;
}

Texture::~Texture() {
  // Deallocate
  free();
}

bool Texture::loadFromFile(Window &window, const std::string &path) {
  // Get rid of preexisting texture
  free();

  // The final texture
  SDL_Texture *newTexture = nullptr;

  // Load image at specified path
  SDL_Surface *loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == nullptr) {
	std::cout << "Unable to load image " << path.c_str() << "! SDL_image Error: "
			  << IMG_GetError() << std::endl;
  } else {
	// Color key image
	SDL_SetColorKey(loadedSurface, SDL_TRUE,
					SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

	// Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(window.mRenderer, loadedSurface);
	if (newTexture == nullptr) {
	  std::cout << "Unable to create texture from " << path.c_str()
				<< "! SDL Error: " << SDL_GetError() << std::endl;
	} else {
	  // Get image dimensions
	  mWidth = loadedSurface->w;
	  mHeight = loadedSurface->h;
	}

	// Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);
  }

  // Return success
  mTexture = newTexture;
  return mTexture != nullptr;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool Texture::loadFromRenderedText(Window &window, const std::string &textureText,
								   SDL_Color textColor) {
  // Get rid of preexisting texture
  free();

  // Render text surface
  SDL_Surface *textSurface =
	  TTF_RenderUTF8_Blended_Wrapped(gFont, textureText.c_str(), textColor, 500);
  if (textSurface != nullptr) {
	// Create texture from surface pixels
	mTexture = SDL_CreateTextureFromSurface(window.mRenderer, textSurface);
	if (mTexture == nullptr) {
	  std::cout << "Unable to create texture from rendered text! SDL Error: "
				<< SDL_GetError() << std::endl;
	} else {
	  // Get image dimensions
	  mWidth = textSurface->w;
	  mHeight = textSurface->h;
	}

	// Get rid of old surface
	SDL_FreeSurface(textSurface);
  } else {
	std::cout << "Unable to render text surface! SDL_ttf Error: "
			  << TTF_GetError() << std::endl;
  }

  // Return success
  return mTexture != nullptr;
}
#endif

void Texture::free() {
  // Free texture if it exists
  if (mTexture != nullptr) {
	SDL_DestroyTexture(mTexture);
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
  }
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
  // Modulate texture rgb
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending) {
  // Set blending function
  SDL_SetTextureBlendMode(mTexture, blending);
}

void Texture::setAlpha(Uint8 alpha) {
  // Modulate texture alpha
  SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Texture::render(Window &window, int x, int y, SDL_Rect *clip, double angle,
					 SDL_Point *center, SDL_RendererFlip flip) {
  // Set rendering space and render to screen
  SDL_Rect renderQuad = {x, y, mWidth, mHeight};

  // Set clip rendering dimensions
  if (clip != nullptr) {
	renderQuad.w = clip->w;
	renderQuad.h = clip->h;
  }

  // Render to screen
  SDL_RenderCopyEx(window.mRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth() const { return mWidth; }

int Texture::getHeight() const { return mHeight; }