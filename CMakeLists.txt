cmake_minimum_required(VERSION 3.26)
project(LifeGame)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${project_SOURCE_DIR}/cmake_modules")

add_executable(LifeGame scripts/main.cpp
        scripts/Texture.h
        scripts/Texture.cpp
        scripts/Window.h
        scripts/Window.cpp
        scripts/PixelWindow.h
        scripts/PixelWindow.cpp)

set(SDL2_PATH SDL2)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES})
