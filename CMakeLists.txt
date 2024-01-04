cmake_minimum_required(VERSION 3.26)
project(LifeGame)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(include/SDL2)
link_directories(lib)

file(GLOB_RECURSE LifeGame_SOURCES "scripts/*.cpp")
file(GLOB_RECURSE LifeGame_HEADERS "scripts/*.h")

add_executable(${PROJECT_NAME} ${LifeGame_SOURCES} ${LifeGame_HEADERS}
        scripts/GlobalData.h)

target_link_libraries(${PROJECT_NAME} SDL2-2.0.0 SDL2_image-2.0.0 SDL2_ttf-2.0.0)
