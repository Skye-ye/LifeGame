cmake_minimum_required(VERSION 3.26)
project(LifeGame)

set(CMAKE_CXX_STANDARD 23)

set(SDL_H /opt/homebrew/Cellar/sdl2/2.28.5/include)
include_directories(${SDL_H})

set(SDL_LIB /opt/homebrew/Cellar/sdl2/2.28.5/lib/libSDL2-2.0.0.dylib)
link_libraries(${SDL_LIB})

add_executable(LifeGame main.cpp)
