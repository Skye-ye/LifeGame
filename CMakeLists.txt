cmake_minimum_required(VERSION 3.26)
project(LifeGame)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

add_executable(LifeGame main.cpp
        screen.h
        screen.cpp)

set(SDL2_PATH SDL2)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})


