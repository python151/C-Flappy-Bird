#include <stdio.h> /* printf and fprintf */

#ifdef _WIN32
#include <SDL/SDL.h> /* Windows-specific SDL2 library */
#else
#include <SDL2/SDL.h> /* macOS- and GNU/Linux-specific */
#endif

/* Sets constants */
#define WIDTH 800
#define HEIGHT 600
#define DELAY 3000

int main (int argc, char **argv)
{
    /* Initialises data */
    SDL_Window *window = NULL;
    
    /*
    * Initialises the SDL video subsystem (as well as the events subsystem).
    * Returns 0 on success or a negative error code on failure using SDL_GetError().
    */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
        return 1;
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
        return 1;
    }

    // Setup renderer
    SDL_Renderer* renderer = NULL;
    renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

    // Set render color to red ( background will be rendered in this color )
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

    // Clear winow
    SDL_RenderClear( renderer );

    // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
    SDL_Rect r;
    r.x = 0;
    r.y = HEIGHT-50;
    r.w = 50;
    r.h = 50;

    // Set render color to blue ( rect will be rendered in this color )
    SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );

    // Render rect
    SDL_RenderFillRect( renderer, &r );

    // Render the rect to the screen
    SDL_RenderPresent(renderer);


    /* Pauses all SDL subsystems for a variable amount of milliseconds */
    SDL_Delay(DELAY);

    /* Frees memory */
    SDL_DestroyWindow(window);
    
    /* Shuts down all SDL subsystems */
    SDL_Quit(); 
    
    return 0;
}