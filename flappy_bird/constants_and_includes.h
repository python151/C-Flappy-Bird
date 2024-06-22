#ifndef CONSTANTSANDINCLUDES
#define CONSTANTSANDINCLUDES 1

#include <stdio.h> /* printf and fprintf */
#include <stdbool.h>

#ifdef _WIN32
#include <SDL/SDL.h> /* Windows-specific SDL2 library */
#else
#include <SDL2/SDL.h> /* macOS- and GNU/Linux-specific */
#endif

/* Sets constants */
#define WIDTH 800
#define HEIGHT 600
#define DELAY 300
#define GAME_OBJECTS 2

#endif