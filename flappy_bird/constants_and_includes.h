#ifndef CONSTANTSANDINCLUDES
#define CONSTANTSANDINCLUDES 1

#include <stdio.h> /* printf and fprintf */
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <SDL/SDL.h> /* Windows-specific SDL2 library */
#else
#include <SDL2/SDL.h> /* macOS- and GNU/Linux-specific */
#endif

/* Sets constants */
#define WIDTH 800
#define HEIGHT 600
#define NUMBER_OF_PIPES 5
#define PIPE_WIDTH 50
#define GAME_OBJECTS (2 + 2*NUMBER_OF_PIPES)
#define PIXEL_PER_METER .5
#define FPS 60
#define FRAME_DELAY (1000 / FPS)
#define GRAVITY (PIXEL_PER_METER * 9.8 / FPS)
#define PIPE_GAP 300
#define MINIMUM_PIPE_LENGTH 50
#define MAX_PIPE_TO_PIPE_DISTANCE 400
#define MIN_PIPE_TO_PIPE_DISTANCE 200
#define MAX_BIRD_VELOCITY_Y 20

#endif