# Conway's Game of Life
## Description
This is a simple implementation of Conway's Game of Life in Python. It is a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input. One interacts with the Game of Life by creating an initial configuration and observing how it evolves. The rules of the game are as follows:
1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
5. These rules, which compare the behavior of the automaton to real life, can be condensed into the following:
    - Any live cell with two or three neighbors survives.
    - Any dead cell with three live neighbors becomes a live cell.
    - All other live cells die in the next generation. Similarly, all other dead cells stay dead.
## Usage
To run the program, simply run the following command in the terminal:
### in Mac/Linux:
```bash
./LifeGame
```
### in Windows:
```bash
./LifeGame.exe
```
## Build
To build the program yourself or change the source code, you can either use CMake or GNU Make which are both included in the repository.
### CMake
Check the [CMakeLists.txt](CMakeLists.txt) file and cmake_modules folder for more information.
### GNU Make
Simply run the following command in the terminal:
```bash
make
```
use the following command to clean the build files:
```bash
make cleanall
```
this for cleaning the object files:
```bash
make cleanobj
```
and this for cleaning the executable file:
```bash
make cleanexe
```
Check the [Makefile](Makefile) file for more information.