#ifndef LIFEGAME_SCRIPTS_GLOBALDATA_H_
#define LIFEGAME_SCRIPTS_GLOBALDATA_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// Window size
const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 720;

// Initial conditions
extern int UPDATE_RATE;
extern int LOWER_BOUND;
extern int UPPER_BOUND;

extern TTF_Font *gFont;

#endif //LIFEGAME_SCRIPTS_GLOBALDATA_H_
