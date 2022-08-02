cmake_minimum_required(VERSION 3.22)
project(SearchBinaryTree C)

set(CMAKE_C_STANDARD 99)

set(GFX_FLAGS "-lSDL -lSDL_gfx -lSDL_ttf")
set(COMPILE_FLAGS -Wall -Wextra -pedantic -std=c99 m -O2)

add_executable(SearchBinaryTree main.c gfx/gfx.c gfx/gfx.h)

target_link_libraries(SearchBinaryTree ${GFX_FLAGS} ${COMPILE_FLAGS})
