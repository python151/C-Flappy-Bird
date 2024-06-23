#include "constants_and_includes.h"

#include "./renderer.c"
#include "./GameObject.c"
#include "./objects.c"

int main (int argc, char **argv)
{
    /* Initialises data */
    SDL_Window *window = create_window();
    SDL_Renderer *renderer = create_renderer(window);
    

    // Set render color to red ( background will be rendered in this color )
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 0 );


    // NOTE: When dealing with more dynamic games, further optimization is required
    GameObject* objects = initialize_game_objects(renderer);
    puts("Game objects initialized...");


    bool running = true;
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;

    puts("Starting game loop...");
    while (running) {
        puts("Getting ticks...");
        frameStart = SDL_GetTicks();
        puts("Getting ticks... done");

        puts("Handling events...");
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // Update game state
            for (int i = 0; i < GAME_OBJECTS; i++) {
                printf("Updating game state on event for object #%d...\n", i);
                objects[i].update(&(objects[i]), &event);
                printf("Updating game state on event for object #%d... done\n", i);
            }
        }
        puts("Handling events... done");

        // Update game state
        for (int i = 0; i < GAME_OBJECTS; i++) {
            printf("Updating game state for object #%d...\n", i);
            objects[i].update(&(objects[i]), NULL);
        }

        // Check for collisions
        // NOTE: When working with larger amounts of objects, this must be optimized.
        for (int i = 0; i < GAME_OBJECTS; i++) {
            for (int j = i + 1; j < GAME_OBJECTS; j++) {
                if (SDL_HasIntersection(&(objects[i].rect), &(objects[j].rect))) {
                    objects[i].handle_collision(&(objects[i]),&(objects[j]));
                    objects[j].handle_collision(&(objects[j]), &(objects[i]));
                }
            }
        }

        // Render
        SDL_RenderClear(renderer);
        for (int i = 0; i < GAME_OBJECTS; i++) {
            SDL_RenderCopy(renderer, objects[i].texture, NULL, &objects[i].rect);
        }
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;

        // If frame finished early, delay to maintain the target frame rate
        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    /* Frees memory */
    SDL_DestroyWindow(window);
    free(objects);
    
    /* Shuts down all SDL subsystems */
    SDL_Quit(); 
    
    return 0;
}