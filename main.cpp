#include <array>
#include <random>
#include <algorithm>
#include "screen.h"

const int GAME_WIDTH = 960;
const int GAME_HEIGHT = 540;

bool isAlive(std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> &game, const int x, const int y) {
  int alive = 0;
  // testing the left
  if (x > 0 && game[x - 1][y] == 1) alive += 1;
  // testing the right
  if (x < GAME_WIDTH && game[x + 1][y] == 1) alive += 1;
  // testing top
  if (y > 0 && game[x][y - 1] == 1) alive += 1;
  // testing bottom
  if (y < GAME_HEIGHT && game[x][y + 1] == 1) alive += 1;

  if (y > 0 && x > 0 && game[x - 1][y - 1] == 1) alive += 1;
  // testing top right
  if (y > 0 && x < GAME_WIDTH && game[x + 1][y - 1] == 1)alive += 1;
  // testing bottom left
  if (y < GAME_HEIGHT && x > 0 && game[x - 1][y + 1] == 1) alive += 1;
  // testing bottom right
  if (y < GAME_HEIGHT && x < GAME_WIDTH && game[x + 1][y + 1] == 1) alive += 1;

  // alive and fewer than 2 die
  if (game[x][y] == 1 && alive < 2) return false;
  // alive and 2 or 3 then live
  if (game[x][y] == 1 && (alive == 2 || alive == 3)) return true;
  // more than 3 live die
  if (alive > 3) return false;
  // 3 alive and point is dead, come to life
  if (game[x][y] == 0 && alive == 3) return true;

  return false;
}
int main(int argc, char *argv[]) {
  // import random
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 100);

  // Create screen
  Screen screen;
  std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> display{};
  std::array<std::array<int, GAME_HEIGHT>, GAME_WIDTH> swap{};

  //Create random points
  for (auto &row : display)
	std::generate(row.begin(), row.end(), [&]() { return (dist(rng)) % 5 == 0 ? 1 : 0; });

  // Start game loop
  while (true) {
	// Check for alive points
	for (int i = 0; i < GAME_WIDTH; ++i)
	  for (int j = 0; j < GAME_HEIGHT; ++j)
		swap[i][j] = isAlive(display, i, j) ? 1 : 0;

	//Draw
	for (int i = 0; i < GAME_WIDTH; ++i)
	  for (int j = 0; j < GAME_HEIGHT; ++j) {
		if (swap[i][j]) {
		  screen.drawpixel(static_cast<float>(i), static_cast<float>(j));
		}
	  }

	// Swap buffers
	std::copy(swap.begin(), swap.end(), display.begin());

	//Display to screen
	screen.update();
	SDL_Delay(10);
	screen.input();
	screen.clearpixels();
  }
}

