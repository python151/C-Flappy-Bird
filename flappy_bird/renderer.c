#include "./constants_and_includes.h"

SDL_Window* create_window() {
    /* Initialises data */
    SDL_Window *window = NULL;
    
    /*
    * Initialises the SDL video subsystem (as well as the events subsystem).
    * Returns 0 on success or a negative error code on failure using SDL_GetError().
    */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
        exit(1);
    }

    /* Creates a SDL window */
    window = SDL_CreateWindow("SDL Example", /* Title of the SDL window */
                    SDL_WINDOWPOS_UNDEFINED, /* Position x of the window */
                    SDL_WINDOWPOS_UNDEFINED, /* Position y of the window */
                    WIDTH, /* Width of the window in pixels */
                    HEIGHT, /* Height of the window in pixels */
                    0); /* Additional flag(s) */

    /* Checks if window has been created; if not, exits program */
    if (window == NULL) {
        fprintf(stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
        exit(1);
    }

    return window;
}

SDL_Renderer* create_renderer(SDL_Window* window) {
    // Setup renderer
    SDL_Renderer* renderer = NULL;
    renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

    return renderer;
}
