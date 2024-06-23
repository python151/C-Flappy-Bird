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
#define GAME_OBJECTS 5
#define PIXEL_PER_METER .5
#define FPS 60
#define FRAME_DELAY (1000 / FPS)
#define GRAVITY (PIXEL_PER_METER * 9.8 / FPS)
#define PIPE_GAP 100

#endif